<?php
// dev_api/pool.php
require_once 'db.php';

// URL'den gelen istasyon ID'sini alıyoruz (?sid=45)
$sid = isset($_GET['sid']) ? $mysqli->real_escape_string($_GET['sid']) : '';

if (empty($sid))
{
    echo "0";
    exit;
}

// 1. ADIM: Cihazın durumunu anlık takip edebilmek için ledresult tablosunu şu anki zamanla güncelliyoruz
$nowDate = date("Y-m-d H:i:s");
$sqlUpdate = "UPDATE ledresult SET resdate = '{$nowDate}' WHERE stationid = '{$sid}'";
$mysqli->query($sqlUpdate);

// 2. ADIM: Bu istasyon için web panelinden yeni bir fiyat/ayar emri verilmiş mi (ptype kontrolü)
// Eğer ptype = 'pr' (Price) veya 'pm' (Parameter) gibi bir emir bekliyorsa ve henüz onaylanmadıysa
$sqlCheck = "SELECT ptype FROM ledresult WHERE stationid = '{$sid}' ORDER BY id DESC LIMIT 1";
$result = $mysqli->query($sqlCheck);

if ($result && $result->num_rows > 0)
{
    $row = $result->fetch_assoc();

    // Eğer ptype boş değilse (Örn: 'pr' veya 'pm' ise) bu bir emirdir, ESP32'yi uyandırıyoruz
    if (!empty($row['ptype']))
    {
        echo "1";
    }
    else
    {
        echo "0";
    }
}
else
{
    echo "0";
}

$mysqli->close();