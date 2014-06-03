<?php

$filename = "setup.txt";

$iso = "0";
$ex = "auto";
$ss = "0";
$mm = "matrix";
$ev = "0";
$sh = "0";
$co = "0";
$br = "50";
$sa = "0";

If (isset($_GET['action'])){
  If ($_GET['action'] == "submit"){
    $iso = $_GET['iso'];
    $ex = $_GET['ex'];
    $ss = $_GET['ss'];
    $mm = $_GET['mm'];
    $ev = $_GET['ev'];
    $sh = $_GET['sh'];
    $co = $_GET['co'];
    $br = $_GET['br'];
    $sa = $_GET['sa'];
  }
} else {
  $fileHandle = fopen($filename, 'r') or die ("can't open file for reading");

  $pair = fscanf($fileHandle, "%s %s");
  list ($key, $value) = $pair;
  $iso = $value;

  $pair = fscanf($fileHandle, "%s %s");
  list ($key, $value) = $pair;
  $ex = $value;

  $pair = fscanf($fileHandle, "%s %s");
  list ($key, $value) = $pair;
  $ss = $value;

  $pair = fscanf($fileHandle, "%s %s");
  list ($key, $value) = $pair;
  $mm = $value;

  $pair = fscanf($fileHandle, "%s %s");
  list ($key, $value) = $pair;
  $ev = $value;

  $pair = fscanf($fileHandle, "%s %s");
  list ($key, $value) = $pair;
  $sh = $value;

  $pair = fscanf($fileHandle, "%s %s");
  list ($key, $value) = $pair;
  $co = $value;

  $pair = fscanf($fileHandle, "%s %s");
  list ($key, $value) = $pair;
  $br = $value;

  $pair = fscanf($fileHandle, "%s %s");
  list ($key, $value) = $pair;
  $sa = $value;

  fclose($fileHandle);
}

echo '<form action="setup.php" method="get">';

echo '  <select name="iso" size="1" style="width:160px;">';
echo '    <option ';
If ($iso=="0") echo 'selected ';
echo 'value="0">iso auto</option>';
echo '    <option ';
If ($iso=="100") echo 'selected ';
echo 'value="100">iso 100</option>';
echo '    <option ';
If ($iso=="200") echo 'selected ';
echo 'value="200">iso 200</option>';
echo '    <option ';
If ($iso=="400") echo 'selected ';
echo 'value="400">iso 400</option>';
echo '    <option ';
If ($iso=="400") echo 'selected ';
echo 'value="400">iso 400</option>';
echo '    <option ';
If ($iso=="600") echo 'selected ';
echo 'value="600">iso 600</option>';
echo '    <option ';
If ($iso=="800") echo 'selected ';
echo 'value="800">iso 800</option>';
echo '  </select>';

echo '  <select name="ex" size="1" style="width:160px;">';
echo '    <option ';
If ($ex=="auto") echo 'selected ';
echo 'value="auto">ex auto</option>';
echo '    <option ';
If ($ex=="night") echo 'selected ';
echo 'value="night">ex night</option>';
echo '    <option ';
If ($ex=="nightpreview") echo 'selected ';
echo 'value="nightpreview">ex nightpreview</option>';
echo '    <option ';
If ($ex=="backlight") echo 'selected ';
echo 'value="backlight">ex backlight</option>';
echo '    <option ';
If ($ex=="spotlight") echo 'selected ';
echo 'value="spotlight">ex spotlight</option>';
echo '    <option ';
If ($ex=="sports") echo 'selected ';
echo 'value="sports">ex sports</option>';
echo '    <option ';
If ($ex=="snow") echo 'selected ';
echo 'value="snow">ex snow</option>';
echo '    <option ';
If ($ex=="beach") echo 'selected ';
echo 'value="beach">ex beach</option>';
echo '    <option ';
If ($ex=="verylong") echo 'selected ';
echo 'value="verylong">ex verylong</option>';
echo '    <option ';
If ($ex=="fixedfps") echo 'selected ';
echo 'value="fixedfps">ex fixedfps</option>';
echo '    <option ';
If ($ex=="antishake") echo 'selected ';
echo 'value="antishake">ex antishake</option>';
echo '    <option ';
If ($ex=="fireworks") echo 'selected ';
echo 'value="fireworks">ex fireworks</option>';
echo '  </select>';

echo '  <select name="ss" size="1" style="width:160px;">';
echo '    <option ';
If ($ss=="0") echo 'selected ';
echo 'value="0">auto</option>';

echo '    <option ';
If ($ss=="500") echo 'selected ';
echo 'value="500">1/2000 s</option>';

echo '    <option ';
If ($ss=="666") echo 'selected ';
echo 'value="666">1/1500 s</option>';

echo '    <option ';
If ($ss=="800") echo 'selected ';
echo 'value="800">1/1250 s</option>';

echo '    <option ';
If ($ss=="909") echo 'selected ';
echo 'value="909">1/1100 s</option>';

echo '    <option ';
If ($ss=="1000") echo 'selected ';
echo 'value="1000">1/1000 s</option>';

echo '    <option ';
If ($ss=="1333") echo 'selected ';
echo 'value="1333">1/750 s</option>';

echo '    <option ';
If ($ss=="2000") echo 'selected ';
echo 'value="2000">1/500 s</option>';

echo '    <option ';
If ($ss=="2500") echo 'selected ';
echo 'value="2500">1/400 s</option>';

echo '    <option ';
If ($ss=="4000") echo 'selected ';
echo 'value="4000">1/250 s</option>';

echo '    <option ';
If ($ss=="5000") echo 'selected ';
echo 'value="5000">1/200 s</option>';

echo '    <option ';
If ($ss=="8000") echo 'selected ';
echo 'value="8000">1/125 s</option>';

echo '    <option ';
If ($ss=="10000") echo 'selected ';
echo 'value="10000">1/100 s</option>';

echo '    <option ';
If ($ss=="16666") echo 'selected ';
echo 'value="16666">1/60 s</option>';

echo '    <option ';
If ($ss=="20000") echo 'selected ';
echo 'value="20000">1/50 s</option>';

echo '    <option ';
If ($ss=="33333") echo 'selected ';
echo 'value="33333">1/30 s</option>';

echo '    <option ';
If ($ss=="40000") echo 'selected ';
echo 'value="40000">1/25 s</option>';

echo '    <option ';
If ($ss=="66666") echo 'selected ';
echo 'value="66666">1/15 s</option>';

echo '    <option ';
If ($ss=="100000") echo 'selected ';
echo 'value="100000">1/10 s</option>';

echo '    <option ';
If ($ss=="125000") echo 'selected ';
echo 'value="125000">1/8 s</option>';

echo '    <option ';
If ($ss=="250000") echo 'selected ';
echo 'value="250000">1/4 s</option>';

echo '    <option ';
If ($ss=="330000") echo 'selected ';
echo 'value="330000">1/3 s</option>';

echo '  </select>';

echo '  <select name="mm" size="1" style="width:160px;">';
echo '    <option ';
If ($mm=="average") echo 'selected ';
echo 'value="average">mm average</option>';
echo '    <option ';
If ($mm=="spot") echo 'selected ';
echo 'value="spot">mm spot</option>';
echo '    <option ';
If ($mm=="backlit") echo 'selected ';
echo 'value="backlit">mm backlit</option>';
echo '    <option ';
If ($mm=="matrix") echo 'selected ';
echo 'value="matrix">mm matrix</option>';
echo '  </select>';

echo '  <select name="ev" size="1" style="width:160px;">';
for ($i = -10; $i <= 10; $i += 1) {
  echo '    <option ';
  If (intval($ev)==$i) echo 'selected ';
  echo 'value="'.$i.'">ev '.$i.'</option>';
}
echo '  </select>';

echo '  <select name="sh" size="1" style="width:160px;">';
for ($i = -100; $i <= 100; $i += 5) {
  echo '    <option ';
  If (intval($sh)==$i) echo 'selected ';
  echo 'value="'.$i.'">sh '.$i.'</option>';
}
echo '  </select>';

echo '  <select name="co" size="1" style="width:160px;">';
for ($i = -100; $i <= 100; $i += 5) {
  echo '    <option ';
  If (intval($co)==$i) echo 'selected ';
  echo 'value="'.$i.'">co '.$i.'</option>';
}
echo '  </select>';

echo '  <select name="br" size="1" style="width:160px;">';
for ($i = 0; $i <= 100; $i += 5) {
  echo '    <option ';
  If (intval($br)==$i) echo 'selected ';
  echo 'value="'.$i.'">br '.$i.'</option>';
}
echo '  </select>';

echo '  <select name="sa" size="1" style="width:160px;">';
for ($i = -100; $i <= 100; $i += 5) {
  echo '    <option ';
  If (intval($sa)==$i) echo 'selected ';
  echo 'value="'.$i.'">sa '.$i.'</option>';
}
echo '  </select><br/><br/>';

echo '  <input type="submit" value="submit" style="width:160px;">';
echo '  <input type="hidden" name="action" value="submit">';
echo '</form>';

If (isset($_GET['action'])){
  If ($_GET['action'] == "submit"){
    $fileHandle = fopen($filename, 'w') or die ("can't open file for writing");
    fwrite($fileHandle, "iso ".$_GET['iso']."\n");
    fwrite($fileHandle, "ex ".$_GET['ex']."\n");
    fwrite($fileHandle, "ss ".$_GET['ss']."\n");
    fwrite($fileHandle, "mm ".$_GET['mm']."\n");
    fwrite($fileHandle, "ev ".$_GET['ev']."\n");
    fwrite($fileHandle, "sh ".$_GET['sh']."\n");
    fwrite($fileHandle, "co ".$_GET['co']."\n");
    fwrite($fileHandle, "br ".$_GET['br']."\n");
    fwrite($fileHandle, "sa ".$_GET['sa']."\n");
    fclose($fileHandle);
  }
}
?>
