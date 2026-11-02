<?php
// dev_api/process.php
require_once 'db.php';

$sid     = isset($_GET['sid']) ? $mysqli->real_escape_string($_GET['sid']) : '';
$pid     = isset($_GET['pid']) ? $mysqli->real_escape_string($_GET['pid']) : '';
$getcfj  = isset($_GET['getcfj']) ? $_GET['getcfj'] : '';
$procres = isset($_GET['procres']) ? $_GET['procres'] : '';
$savecfj = isset($_GET['savecfj']) ? $_GET['savecfj'] : '';

// -----------------------------------------------------------------------------
// SENARYO 1: ESP32 Konfigürasyon İndiriyor (?getcfj=t&sid=45)
// -----------------------------------------------------------------------------
if ($getcfj == 't' && !empty($sid))
{
    // station tablosundaki ledcfg kolonundan JSON verisini çekiyoruz
    $sql = "SELECT ledcfg FROM station WHERE sid = '{$sid}'";
    $result = $mysqli->query($sql);

    if ($result && $result->num_rows > 0)
    {
        $row = $result->fetch_assoc();
        header('Content-Type: application/json');
        echo $row['ledcfg'];
        exit;
    }
}

// -----------------------------------------------------------------------------
// SENARYO 2: ESP32 Güncellemeyi Aldığını Onaylıyor (?procres=t&pid=12345)
// -----------------------------------------------------------------------------
if ($procres == 't')
{
    $rawPost = file_get_contents('php://input');

    if (strpos($rawPost, 'paramok') !== false)
    {
        // Cihaz paketi aldığını doğruladı.
        // ledresult tablosundaki ptype alanını boşaltarak emrin tamamlandığını işaretliyoruz.
        $nowDate = date("Y-m-d H:i:s");
        $sql = "UPDATE ledresult SET ptype = '', resdate = '{$nowDate}' WHERE processid = '{$pid}'";
        $mysqli->query($sql);

        echo "{\"res\":\"paramok\"}";
        exit;
    }
}

// -----------------------------------------------------------------------------
// SENARYO 3: ESP32 Kendi Panelindeki Ayarı Sunucuya Yedekliyor (?savecfj=t&sid=45)
// -----------------------------------------------------------------------------
if ($savecfj == 't' && !empty($sid))
{
    $rawJson = $mysqli->real_escape_string(file_get_contents('php://input'));

    if (!empty($rawJson))
    {
        // İstasyonun güncel durumunu station tablosundaki ledcfg alanına yazıyoruz
        $sql = "UPDATE station SET ledcfg = '{$rawJson}' WHERE sid = '{$sid}'";
        $mysqli->query($sql);

        echo "{\"res\":\"ok\"}";
        exit;
    }
}

echo "{}";
$mysqli->close();