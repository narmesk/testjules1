<?php
// dev_api/db.php

ini_set('display_errors', 1);
error_reporting(E_ALL);

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

$mysqli = new mysqli($localhost, $dbuser, $dbpasword, $dbname);

if ($mysqli->connect_errno)
{
    echo "Veritabanı bağlantı hatası: " . $mysqli->connect_error;
    exit();
}

$mysqli->set_charset("utf8");
