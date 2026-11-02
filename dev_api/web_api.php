<?php
// dev_api/web_api.php
require_once 'db.php';

ini_set('display_errors', 1);
error_reporting(E_ALL);

header('Content-Type: application/json');

$action = isset($_GET['action']) ? $_GET['action'] : '';
$sid    = isset($_GET['sid']) ? $mysqli->real_escape_string($_GET['sid']) : '';

// -----------------------------------------------------------------------------
// EYLEM 1: İstasyonları ve En Son Görülme Zamanlarını Listeleme (?action=list)
// -----------------------------------------------------------------------------
if ($action == 'list')
{
    $stations = array();

    $sql = "SELECT s.sid, s.name as station_name, s.address, MAX(r.resdate) as last_seen
            FROM station s
            LEFT JOIN ledresult r ON s.sid = r.stationid
            GROUP BY s.sid, s.name, s.address";

    $result = $mysqli->query($sql);

    if ($result)
    {
        while ($row = $result->fetch_assoc())
        {
            // KRİTİK DÜZELTME: Köşeli parantezler eklenerek gerçek bir liste (Array) oluşturuluyor
            $stations[] = $row;
        }
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

    $formPrices = array(
        isset($_POST['p0_cash']) ? $mysqli->real_escape_string($_POST['p0_cash']) : '0.00',
        isset($_POST['p0_credit']) ? $mysqli->real_escape_string($_POST['p0_credit']) : '0.00',
        isset($_POST['p0_debit']) ? $mysqli->real_escape_string($_POST['p0_debit']) : '0.00',

        isset($_POST['p1_cash']) ? $mysqli->real_escape_string($_POST['p1_cash']) : '0.00',
        isset($_POST['p1_credit']) ? $mysqli->real_escape_string($_POST['p1_credit']) : '0.00',
        isset($_POST['p1_debit']) ? $mysqli->real_escape_string($_POST['p1_debit']) : '0.00',

        isset($_POST['p2_cash']) ? $mysqli->real_escape_string($_POST['p2_cash']) : '0.00',
        isset($_POST['p2_credit']) ? $mysqli->real_escape_string($_POST['p2_credit']) : '0.00',
        isset($_POST['p2_debit']) ? $mysqli->real_escape_string($_POST['p2_debit']) : '0.00',

        isset($_POST['p3_cash']) ? $mysqli->real_escape_string($_POST['p3_cash']) : '0.00',
        isset($_POST['p3_credit']) ? $mysqli->real_escape_string($_POST['p3_credit']) : '0.00',
        isset($_POST['p3_debit']) ? $mysqli->real_escape_string($_POST['p3_debit']) : '0.00',

        isset($_POST['p4_cash']) ? $mysqli->real_escape_string($_POST['p4_cash']) : '0.00',
        isset($_POST['p4_credit']) ? $mysqli->real_escape_string($_POST['p4_credit']) : '0.00',
        isset($_POST['p4_debit']) ? $mysqli->real_escape_string($_POST['p4_debit']) : '0.00',

        isset($_POST['p5_cash']) ? $mysqli->real_escape_string($_POST['p5_cash']) : '0.00',
        isset($_POST['p5_credit']) ? $mysqli->real_escape_string($_POST['p5_credit']) : '0.00',
        isset($_POST['p5_debit']) ? $mysqli->real_escape_string($_POST['p5_debit']) : '0.00',

        isset($_POST['p6_cash']) ? $mysqli->real_escape_string($_POST['p6_cash']) : '0.00',
        isset($_POST['p6_credit']) ? $mysqli->real_escape_string($_POST['p6_credit']) : '0.00',
        isset($_POST['p6_debit']) ? $mysqli->real_escape_string($_POST['p6_debit']) : '0.00',

        isset($_POST['p7_cash']) ? $mysqli->real_escape_string($_POST['p7_cash']) : '0.00',
        isset($_POST['p7_credit']) ? $mysqli->real_escape_string($_POST['p7_credit']) : '0.00',
        isset($_POST['p7_debit']) ? $mysqli->real_escape_string($_POST['p7_debit']) : '0.00'
    );

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
    else
    {
        echo json_encode(array("status" => "error", "message" => "Target station not found."));
        exit;
    }
}

// -----------------------------------------------------------------------------
// EYLEM 3: İstasyonun Veritabanındaki Mevcut Fiyatlarını Çekme (?action=get_prices&sid=45)
// -----------------------------------------------------------------------------
if ($action == 'get_prices' && !empty($sid))
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
    else
    {
        echo json_encode(array("status" => "error", "message" => "Station code not found."));
        exit;
    }
}

echo json_encode(array("status" => "error", "message" => "Invalid action."));
$mysqli->close();
?>
