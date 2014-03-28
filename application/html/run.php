<?php
	//echo system("../../executables/NEAT ../../genetics_encodings/test_encoding.genetic_encoding");
	 exec("../../executables/NEAT ../../genetics_encodings/test_encoding.genetic_encoding", $output);
	 for ($i=0; $i < sizeof($output); $i++) { 
	  	echo $output[$i];
	  	echo "<br />";
	  } 
?>