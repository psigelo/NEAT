<?php
	$fp = fopen('data.txt', 'w');
	

	
	fwrite($fp, "#ifndef UDER_DEF_HPP\n#define UDER_DEF_HPP\n");
	fwrite($fp, '#define ');

	
	fwrite($fp, '#endif ');
	fclose($fp);
?>