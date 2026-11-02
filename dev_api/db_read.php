<?php
// dev_api/db_read.php

// Geliştirme aşamasında hataları ve çıktıları görmek için açıyoruz
ini_set('display_errors', 1);
error_reporting(E_ALL);

// Eski sistemdeki veritabanı bilgilerini otomatik çekmek için config.php dahil ediliyor
if (file_exists("../config.php"))
{
    include("../config.php");
}
else if (file_exists("config.php"))
{
    include("config.php");
}

// Zaman dilimi ayarı
date_default_timezone_set(isset($timezone) ? $timezone : 'Europe/Istanbul');

// Eski func.php dosyanızdaki mysqli bağlantı mantığı kuruluyor
$mysqli = new mysqli($localhost, $dbuser, $dbpasword, $dbname);

// Bağlantı kontrolü
if ($mysqli->connect_errno)
{
    echo "Veritabanına bağlanılamadı: " . $mysqli->connect_error;
    exit();
}

// Türkçe karakter uyumu için bağlantı seti ayarlanıyor
$mysqli->set_charset("utf8");

echo "<h2>--- VERİTABANI ŞEMA ANALİZİ ---</h2>";

// 1. ADIM: Veritabanındaki tüm tabloları listeliyoruz
$tablesResult = $mysqli->query("SHOW TABLES");

if ($tablesResult)
{
    while ($tableRow = $tablesResult->fetch_row())
    {
        $tableName = $tableRow[0];
        echo "<br><b>[Tablo]: " . $tableName . "</b><br>";
        echo "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse:collapse; margin-bottom:15px;'>";
        echo "<tr Skinner bg='#eee'><th>Kolon Adı (Field)</th><th>Veri Tipi (Type)</th><th>Boş Kalabilir mi? (Null)</th><th>Varsayılan Değer (Default)</th></tr>";

        // 2. ADIM: Her tablonun içindeki kolonları ve veri tiplerini öğreniyoruz
        $columnsResult = $mysqli->query("SHOW COLUMNS FROM `" . $tableName . "`");

        if ($columnsResult)
        {
            while ($columnRow = $columnsResult->fetch_assoc())
            {
                echo "<tr>";
                echo "<td>" . $columnRow['Field'] . "</td>";
                echo "<td>" . $columnRow['Type'] . "</td>";
                echo "<td>" . $columnRow['Null'] . "</td>";
                echo "<td>" . ($columnRow['Default'] === null ? 'NULL' : $columnRow['Default']) . "</td>";
                echo "</tr>";
            }
        }

        echo "</table>";
    }
}
else
{
    echo "Tablolar listelenirken bir hata oluştu: " . $mysqli->error;
}

// Bağlantıyı güvenli bir şekilde kapatıyoruz
$mysqli->close();
?>
