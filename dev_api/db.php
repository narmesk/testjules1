<?php
// dev_api/db.php - Veritabanı Bağlantı ve Yapılandırma Motoru

ini_set('display_errors', 0);
error_reporting(E_ALL);

// Yapılandırma dosyası dahil etme denemeleri
if (file_exists("config.php"))
{
    include("config.php");
}
else if (file_exists("../config.php"))
{
    include("../config.php");
}
else if (file_exists("../../config.php"))
{
    include("../../config.php");
}

date_default_timezone_set(isset($timezone) ? $timezone : 'Europe/Istanbul');

// Değişkenlerin tanımlı olup olmadığını kontrol ediyoruz
$localhost = isset($localhost) ? $localhost : "127.0.0.1";
$dbuser    = isset($dbuser) ? $dbuser : "root";
$dbpasword = isset($dbpasword) ? $dbpasword : "";
$dbname    = isset($dbname) ? $dbname : "ledovate";

// Güvenli MySQLi bağlantısı (Hata durumunda uygulamanın çökmesini engeller)
$mysqli = null;

try
{
    $mysqli = @new mysqli($localhost, $dbuser, $dbpasword, $dbname);
    if ($mysqli->connect_errno)
    {
        $mysqli = null;
    }
    else
    {
        $mysqli->set_charset("utf8");
    }
}
catch (Throwable $e)
{
    $mysqli = null;
}
