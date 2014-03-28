<html>
	<head>
		<title>
			NEAT-App
		</title>
		<script language="javascript" type="text/javascript" src="./lib/jquery.js"></script>
		<script language="javascript" type="text/javascript" src="./lib/app.js"></script>
		<link rel="stylesheet" type="text/css" href="./style/app_style.css">
		<META HTTP-EQUIV="Content-Type" content="text/html; charset=utf-8"/>
	</head>
	<body>
		<header>
			<center><h1>NEAT</h1></center> 
		</header>

		<div class="menubar">.
			<span class="log-logout"></span> 
			<span class="register" > register </span> 
			<span class="documentation"> Documentation </span> 
			<span class="tutorial"> Tutorial </span> 
			<span class="usertext">  </span>                            
		</div>

		<section class="choose-area cf" >
			<span class="customSelect choose-property mainTaskBar" id="barra" >
				<p id="textbar" style="font-family:'Poiret One'; margin:0px; padding:0px; font-size:25;"></p>
				<span class="block-icon">
					<span></span> 
					<span></span> 
					<span></span> 
					<span></span> 
					<span></span> 
					<span></span>
				</span>
			</span>
		</section>



		<div id="option-boxes" align="center">
			<div class="option-left">
				<div class="option-box option-user" id="choose-genome">Choose genome</div>
				<div class="option-box option-user" id="choose-probabilities">Choose NEAT-probabilities</div>
				<div class="option-box option-user" id="choose-fitness" >Choose a fitness function</div>
				<div id="compile"> Compile </div>
				<div id="run"> Run </div>
			</div>
			<div class="option-right"></div>
		</div>

		<div id="contenido" align="center" style="display:none; clear:both"> </div>
		
		
		
		

	</body>
</html>