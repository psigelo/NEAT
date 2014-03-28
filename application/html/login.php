<?php
	
	$fichero_texto = fopen ("../database/users", "r");

	$contenido_fichero = fgets($fichero_texto);
	$amount_of_lines = (int)$contenido_fichero;

	for ($i=0; $i < $amount_of_lines; $i++) { 
		$contenido_fichero = fgets($fichero_texto);
		echo "<div class=\"option-box usersOptions\"> $contenido_fichero </div>";
	}
	echo "<p> Or you can <span class=\"register\">register</span> quickly </p>";

	fclose($fichero_texto);
?>

