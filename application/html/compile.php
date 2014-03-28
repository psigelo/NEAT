<?php
	 exec("make", $output);
	 for ($i=0; $i < sizeof($output); $i++) { 
	  	echo $output[$i];
	  	echo "<br />";
	  } 
?>