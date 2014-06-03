<?php
  $image = 'preview.jpg';
  $exif = exif_read_data($image, 0 , true);

  echo "<table>";

  echo '<tr><td width="150">ISO</td><td>';
  if(isset($exif["EXIF"]["ISOSpeedRatings"])) {
    echo $exif["EXIF"]["ISOSpeedRatings"];
  }
  echo "</td></tr>";

  echo "<tr><td>Exposure Time</td><td>";
  if (isset($exif["EXIF"]["ExposureTime"])) {
    list($num, $den) = explode("/", $exif["EXIF"]["ExposureTime"]);
    if ($num > $den) {
      echo "{$num} s";
    } else {
      $den = round($den/$num);
      echo "1/{$den} s";
    }
  }
  echo "</td></tr>";

  echo "<tr><td>F-Number</td><td>";
  if (isset($exif["EXIF"]["FNumber"])) {
    list($num, $den) = explode("/",$exif["EXIF"]["FNumber"]);
    echo "F/" . round(10.0*$num/$den)/10.0;
  }
  echo "</td></tr>";

  echo "<tr><td>Focal Length</td><td>";
  if (isset($exif["EXIF"]["FocalLength"])) {
    list($num, $den) = explode("/", $exif["EXIF"]["FocalLength"]);
    echo round(100.0*$num/$den)/100.0 . " mm";
  }
  echo "</td></tr>";

  echo "</table>";

  //foreach ($exif as $key => $section) {
  //  foreach ($section as $name => $val) {
  //    echo "$key.$name: $val <br/>\n";
  //  }
  //}
?>
