<?php
// dev_api/web_api.php - İstasyon Yönetim ve Senkronizasyon API Motoru
require_once 'db.php';

ini_set('display_errors', 0);
error_reporting(E_ALL);

header('Content-Type: application/json; charset=utf-8');

$action = isset($_GET['action']) ? $_GET['action'] : '';
$sid    = isset($_GET['sid']) ? ($mysqli ? $mysqli->real_escape_string($_GET['sid']) : preg_replace('/[^a-zA-Z0-9_-]/', '', $_GET['sid'])) : '';

// -----------------------------------------------------------------------------
// EYLEM 1: İstasyonları ve En Son Görülme Zamanlarını Listeleme (?action=list)
// -----------------------------------------------------------------------------
if ($action == 'list')
{
    $stations = array();

    if ($mysqli)
    {
        $sql = "SELECT s.sid, s.name as station_name, s.address, MAX(r.resdate) as last_seen
                FROM station s
                LEFT JOIN ledresult r ON s.sid = r.stationid
                GROUP BY s.sid, s.name, s.address";

        $result = $mysqli->query($sql);

        if ($result)
        {
            while ($row = $result->fetch_assoc())
            {
                $stations[] = $row;
            }
        }
    }

    // Veritabanı boşsa veya bağlı değilse gösterim için mock istasyon döndür
    if (empty($stations))
    {
        $stations[] = array(
            "sid" => "demo1",
            "station_name" => "Demo Station Central",
            "address" => "100 Automation Blvd, Istanbul",
            "last_seen" => date("Y-m-d H:i:s")
        );
    }

    echo json_encode($stations, JSON_UNESCAPED_UNICODE);
    exit;
}

// -----------------------------------------------------------------------------
// EYLEM 2: Panelden Gelen 24 Fiyatı ve Parametreleri Kuyruğa İşleme (?action=update&sid=45)
// -----------------------------------------------------------------------------
if ($action == 'update' && !empty($sid))
{
    $brightness = isset($_POST['brightness']) ? $_POST['brightness'] : '10';
    $speed      = isset($_POST['speed']) ? $_POST['speed'] : '8';

    // Fiyat matrisini hem FULLVAL hem de eski p0..p7 formatlarına uyumlu olarak topluyoruz
    $formPrices = array();

    for ($i = 0; $i < 8; $i++)
    {
        $pnum = $i + 1;

        // Cash (A)
        if (isset($_POST["FULLVAL{$pnum}A"]))
        {
            $valA = $_POST["FULLVAL{$pnum}A"];
        }
        else if (isset($_POST["p{$i}_cash"]))
        {
            $valA = $_POST["p{$i}_cash"];
        }
        else
        {
            $valA = '0.00';
        }

        // Credit (B)
        if (isset($_POST["FULLVAL{$pnum}B"]))
        {
            $valB = $_POST["FULLVAL{$pnum}B"];
        }
        else if (isset($_POST["p{$i}_credit"]))
        {
            $valB = $_POST["p{$i}_credit"];
        }
        else
        {
            $valB = '0.00';
        }

        // Debit (C)
        if (isset($_POST["FULLVAL{$pnum}C"]))
        {
            $valC = $_POST["FULLVAL{$pnum}C"];
        }
        else if (isset($_POST["p{$i}_debit"]))
        {
            $valC = $_POST["p{$i}_debit"];
        }
        else
        {
            $valC = '0.00';
        }

        $formPrices[] = $mysqli ? $mysqli->real_escape_string($valA) : $valA;
        $formPrices[] = $mysqli ? $mysqli->real_escape_string($valB) : $valB;
        $formPrices[] = $mysqli ? $mysqli->real_escape_string($valC) : $valC;
    }

    // Ayrıca prices.json dosyasını da güncelleyip kalıcı hale getiriyoruz
    $pricesPath = __DIR__ . '/jsons/prices.json';
    if (file_exists($pricesPath))
    {
        $curPrices = json_decode(file_get_contents($pricesPath), true);
        if (!is_array($curPrices))
        {
            $curPrices = array();
        }
        foreach ($_POST as $k => $v)
        {
            if (strpos($k, 'VAL') !== false)
            {
                $curPrices[$k] = $v;
            }
        }
        file_put_contents($pricesPath, json_encode($curPrices, JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE));
    }

    if ($mysqli)
    {
        $sqlSelect = "SELECT ledcfg FROM station WHERE sid = '{$sid}'";
        $resSelect = $mysqli->query($sqlSelect);

        if ($resSelect && $resSelect->num_rows > 0)
        {
            $row = $resSelect->fetch_assoc();
            $configArray = json_decode($row['ledcfg'], true);

            if (!is_array($configArray))
            {
                $configArray = array(
                    "sid" => $sid,
                    "pid" => "00000000",
                    "ptype" => "st",
                    "pmaster" => "sv",
                    "pdate" => date("Y-m-d H:i:s"),
                    "leds" => array(),
                    "parameters" => array()
                );
            }

            for ($i = 0; $i < 24; $i++)
            {
                $configArray['leds'][$i] = array(
                    "v" => $formPrices[$i]
                );
            }

            $configArray['parameters'][0] = array("v" => strval($brightness));
            $configArray['parameters'][1] = array("v" => strval($speed));

            $processId = strval(rand(11111111, 99999999));
            $configArray['ptype']   = "pr";
            $configArray['pmaster'] = "sv";
            $configArray['pid']     = $processId;
            $configArray['pdate']   = date("Y-m-d H:i:s");

            $updatedJson = $mysqli->real_escape_string(json_encode($configArray, JSON_UNESCAPED_UNICODE));

            $sqlUpdateStation = "UPDATE station SET ledcfg = '{$updatedJson}' WHERE sid = '{$sid}'";
            $mysqli->query($sqlUpdateStation);

            $sqlUpdateResult = "UPDATE ledresult SET ptype = 'pr', processid = '{$processId}' WHERE stationid = '{$sid}'";
            $mysqli->query($sqlUpdateResult);

            echo json_encode(array("status" => "success", "message" => "Matrix configuration successfully queued."));
            exit;
        }
    }

    echo json_encode(array("status" => "success", "message" => "Configuration successfully saved."));
    exit;
}

// -----------------------------------------------------------------------------
// EYLEM 3: İstasyonun Veritabanındaki Mevcut Fiyatlarını Çekme (?action=get_prices&sid=45)
// -----------------------------------------------------------------------------
if ($action == 'get_prices' && !empty($sid))
{
    if ($mysqli)
    {
        $sqlSelect = "SELECT ledcfg FROM station WHERE sid = '{$sid}'";
        $resSelect = $mysqli->query($sqlSelect);

        if ($resSelect && $resSelect->num_rows > 0)
        {
            $row = $resSelect->fetch_assoc();
            $configArray = json_decode($row['ledcfg'], true);

            $prices = array();

            if (isset($configArray['leds']) && is_array($configArray['leds']))
            {
                for ($i = 0; $i < 24; $i++)
                {
                    if (isset($configArray['leds'][$i]['v']))
                    {
                        $prices[$i] = $configArray['leds'][$i]['v'];
                    }
                    else
                    {
                        $prices[$i] = "0.00";
                    }
                }
            }
            else
            {
                $prices = array_fill(0, 24, "0.00");
            }

            echo json_encode(array("status" => "success", "prices" => $prices));
            exit;
        }
    }

    $pricesPath = __DIR__ . '/jsons/prices.json';
    if (file_exists($pricesPath))
    {
        $pricesData = json_decode(file_get_contents($pricesPath), true);
        echo json_encode(array("status" => "success", "full_self" => $pricesData));
        exit;
    }

    echo json_encode(array("status" => "success", "prices" => array_fill(0, 24, "0.00")));
    exit;
}

// -----------------------------------------------------------------------------
// EYLEM 4: İstasyonun Tuning Ofset Değerlerini Çekme (?action=get_tuning&sid=45)
// -----------------------------------------------------------------------------
if ($action == 'get_tuning')
{
    $tuningPath = __DIR__ . '/jsons/tunning.json';
    if (file_exists($tuningPath))
    {
        $jsonContent = file_get_contents($tuningPath);
        $tuningData = json_decode($jsonContent, true);
        echo json_encode(array("status" => "success", "tuning" => $tuningData));
        exit;
    }
    else
    {
        echo json_encode(array("status" => "error", "message" => "Tuning configuration not found."));
        exit;
    }
}

// -----------------------------------------------------------------------------
// EYLEM 5: İstasyonun Ürün İsimleri ve Renk Konfigürasyonunu Çekme (?action=get_configs)
// -----------------------------------------------------------------------------
if ($action == 'get_configs')
{
    $configsPath = __DIR__ . '/jsons/configs.json';
    if (file_exists($configsPath))
    {
        $jsonContent = file_get_contents($configsPath);
        $configsData = json_decode($jsonContent, true);
        echo json_encode(array("status" => "success", "configs" => $configsData));
        exit;
    }
    else
    {
        echo json_encode(array("status" => "error", "message" => "Configs template not found."));
        exit;
    }
}

// -----------------------------------------------------------------------------
// EYLEM 6: Ürün İsimleri ve Renk Konfigürasyonunu Kaydetme (?action=update_configs)
// -----------------------------------------------------------------------------
if ($action == 'update_configs')
{
    $configsPath = __DIR__ . '/jsons/configs.json';
    $currentData = array();
    if (file_exists($configsPath))
    {
        $currentData = json_decode(file_get_contents($configsPath), true);
    }

    foreach ($_POST as $key => $val)
    {
        $currentData[$key] = $val;
    }

    file_put_contents($configsPath, json_encode($currentData, JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE));
    echo json_encode(array("status" => "success", "message" => "Configs updated successfully."));
    exit;
}

// -----------------------------------------------------------------------------
// EYLEM 7: Tuning Ofset Konfigürasyonunu Kaydetme (?action=update_tuning)
// -----------------------------------------------------------------------------
if ($action == 'update_tuning')
{
    $tuningPath = __DIR__ . '/jsons/tunning.json';
    $currentData = array();
    if (file_exists($tuningPath))
    {
        $currentData = json_decode(file_get_contents($tuningPath), true);
    }

    foreach ($_POST as $key => $val)
    {
        $currentData[$key] = $val;
    }

    file_put_contents($tuningPath, json_encode($currentData, JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE));
    echo json_encode(array("status" => "success", "message" => "Tuning offsets updated successfully."));
    exit;
}

// -----------------------------------------------------------------------------
// EYLEM 8: Donanım Ayarlarını Çekme (?action=get_settings)
// -----------------------------------------------------------------------------
if ($action == 'get_settings')
{
    $brightness = "33";
    $speed = "27";
    $settingsData = array();

    $settingsPath = __DIR__ . '/jsons/settings.json';
    if (file_exists($settingsPath))
    {
        $settingsData = json_decode(file_get_contents($settingsPath), true);
        if (!is_array($settingsData))
        {
            $settingsData = array();
        }
    }

    if ($mysqli && !empty($sid))
    {
        $sqlSelect = "SELECT ledcfg FROM station WHERE sid = '{$sid}'";
        $resSelect = $mysqli->query($sqlSelect);
        if ($resSelect && $resSelect->num_rows > 0)
        {
            $row = $resSelect->fetch_assoc();
            $cfg = json_decode($row['ledcfg'], true);
            if (isset($cfg['parameters'][0]['v']))
            {
                $brightness = $cfg['parameters'][0]['v'];
            }
            if (isset($cfg['parameters'][1]['v']))
            {
                $speed = $cfg['parameters'][1]['v'];
            }
        }
    }

    echo json_encode(array(
        "status" => "success",
        "brightness" => $brightness,
        "speed" => $speed,
        "settings" => $settingsData
    ));
    exit;
}

// -----------------------------------------------------------------------------
// EYLEM: Genel Sistem Ayarlarını Kaydetme (?action=update_settings)
// -----------------------------------------------------------------------------
if ($action == 'update_settings')
{
    $settingsPath = __DIR__ . '/jsons/settings.json';
    $settingsData = array();
    if (file_exists($settingsPath))
    {
        $settingsData = json_decode(file_get_contents($settingsPath), true);
        if (!is_array($settingsData))
        {
            $settingsData = array();
        }
    }

    foreach ($_POST as $key => $val)
    {
        $settingsData[$key] = $val;
    }

    file_put_contents($settingsPath, json_encode($settingsData, JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE));
    echo json_encode(array("status" => "success", "message" => "Settings updated successfully."));
    exit;
}

// -----------------------------------------------------------------------------
// EYLEM 10: PT Konfigürasyonu ve Donanım Parametrelerini Çekme (?action=get_pt_config)
// -----------------------------------------------------------------------------
if ($action == 'get_pt_config')
{
    $brightness = "33";
    $speed = "27";
    $ptList = array();

    $ptlsPath = __DIR__ . '/jsons/ptls.json';
    if (file_exists($ptlsPath))
    {
        $ptList = json_decode(file_get_contents($ptlsPath), true);
        if (!is_array($ptList))
        {
            $ptList = array();
        }
    }

    if ($mysqli && !empty($sid))
    {
        $sqlSelect = "SELECT ledcfg FROM station WHERE sid = '{$sid}'";
        $resSelect = $mysqli->query($sqlSelect);
        if ($resSelect && $resSelect->num_rows > 0)
        {
            $row = $resSelect->fetch_assoc();
            $cfg = json_decode($row['ledcfg'], true);
            if (isset($cfg['parameters'][0]['v']))
            {
                $brightness = $cfg['parameters'][0]['v'];
            }
            if (isset($cfg['parameters'][1]['v']))
            {
                $speed = $cfg['parameters'][1]['v'];
            }
            if (isset($cfg['ptlist']) && is_array($cfg['ptlist']))
            {
                $ptList = array_merge($ptList, $cfg['ptlist']);
            }
        }
    }

    echo json_encode(array(
        "status" => "success",
        "brightness" => $brightness,
        "speed" => $speed,
        "pt_list" => $ptList
    ));
    exit;
}

// -----------------------------------------------------------------------------
// EYLEM 11: PT Konfigürasyonu ve Donanım Parametrelerini Güncelleme (?action=update_pt_config)
// -----------------------------------------------------------------------------
if ($action == 'update_pt_config' && !empty($sid))
{
    $brightness = isset($_POST['brightness']) ? $_POST['brightness'] : '33';
    $speed      = isset($_POST['speed']) ? $_POST['speed'] : '27';

    $ptlsPath = __DIR__ . '/jsons/ptls.json';
    $currentPtData = array();
    if (file_exists($ptlsPath))
    {
        $currentPtData = json_decode(file_get_contents($ptlsPath), true);
        if (!is_array($currentPtData))
        {
            $currentPtData = array();
        }
    }

    // Gelen PT alanlarını JSON'a işle
    foreach ($_POST as $key => $val)
    {
        if ($key != 'brightness' && $key != 'speed')
        {
            $currentPtData[$key] = $val;
        }
    }

    file_put_contents($ptlsPath, json_encode($currentPtData, JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE));

    if ($mysqli)
    {
        $sqlSelect = "SELECT ledcfg FROM station WHERE sid = '{$sid}'";
        $resSelect = $mysqli->query($sqlSelect);

        if ($resSelect && $resSelect->num_rows > 0)
        {
            $row = $resSelect->fetch_assoc();
            $configArray = json_decode($row['ledcfg'], true);

            if (!is_array($configArray))
            {
                $configArray = array();
            }

            $configArray['parameters'][0] = array("v" => strval($brightness));
            $configArray['parameters'][1] = array("v" => strval($speed));
            $configArray['ptlist'] = $currentPtData;

            $processId = strval(rand(11111111, 99999999));
            $configArray['ptype']   = "pm";
            $configArray['pmaster'] = "sv";
            $configArray['pid']     = $processId;
            $configArray['pdate']   = date("Y-m-d H:i:s");

            $updatedJson = $mysqli->real_escape_string(json_encode($configArray, JSON_UNESCAPED_UNICODE));
            $mysqli->query("UPDATE station SET ledcfg = '{$updatedJson}' WHERE sid = '{$sid}'");
            $mysqli->query("UPDATE ledresult SET ptype = 'pm', processid = '{$processId}' WHERE stationid = '{$sid}'");

            echo json_encode(array("status" => "success", "message" => "PT configuration and hardware parameters queued."));
            exit;
        }
    }

    echo json_encode(array("status" => "success", "message" => "PT configuration successfully saved."));
    exit;
}

// -----------------------------------------------------------------------------
// EYLEM 9: Donanım Ayarlarını Güncelleme (?action=update_hardware)
// -----------------------------------------------------------------------------
if ($action == 'update_hardware' && !empty($sid))
{
    $brightness = isset($_POST['brightness']) ? $_POST['brightness'] : '33';
    $speed      = isset($_POST['speed']) ? $_POST['speed'] : '27';

    if ($mysqli)
    {
        $sqlSelect = "SELECT ledcfg FROM station WHERE sid = '{$sid}'";
        $resSelect = $mysqli->query($sqlSelect);

        if ($resSelect && $resSelect->num_rows > 0)
        {
            $row = $resSelect->fetch_assoc();
            $configArray = json_decode($row['ledcfg'], true);

            if (!is_array($configArray))
            {
                $configArray = array();
            }

            $configArray['parameters'][0] = array("v" => strval($brightness));
            $configArray['parameters'][1] = array("v" => strval($speed));

            $processId = strval(rand(11111111, 99999999));
            $configArray['ptype']   = "pm";
            $configArray['pmaster'] = "sv";
            $configArray['pid']     = $processId;
            $configArray['pdate']   = date("Y-m-d H:i:s");

            $updatedJson = $mysqli->real_escape_string(json_encode($configArray, JSON_UNESCAPED_UNICODE));
            $mysqli->query("UPDATE station SET ledcfg = '{$updatedJson}' WHERE sid = '{$sid}'");
            $mysqli->query("UPDATE ledresult SET ptype = 'pm', processid = '{$processId}' WHERE stationid = '{$sid}'");

            echo json_encode(array("status" => "success", "message" => "Hardware parameters queued."));
            exit;
        }
    }

    echo json_encode(array("status" => "success", "message" => "Hardware parameters queued successfully."));
    exit;
}

echo json_encode(array("status" => "error", "message" => "Invalid action."));
if ($mysqli)
{
    $mysqli->close();
}
?>
