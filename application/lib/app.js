var flag_register = 0;

function regist(){
	flag_register = 1;
		$.ajax({
			  url: "./html/register-form.php",
			  cache: false
			})
			  .done(function( html ) {
				$( "#contenido" ).addClass("desarrollo");
			  	$( "#contenido" ).empty();
				$( "#contenido" ).append(html);
			    $( "#contenido" ).fadeIn();
			    $("#sent_registration").on("click",function(){
					
			    	
			    	$.ajax({
						  type: "POST",
						  url: "./html/register.php",
						  data: { user: $("#usr2reg").val()}
						})
						  .done(function( msg ) {
						    
						  });
	

						sessionStorage.setItem("user", $("#usr2reg").val());
						$(".usertext").text(sessionStorage.getItem("user"));
						$(".log-logout").text("log-out");
						$("#textbar").text("Start train");
						$("#textbar").closest("#barra").removeClass("choose-property").removeClass("customSelect").addClass("choose-property2").addClass("customSelect2");

						$( "#contenido" ).empty();
						$( "#contenido" ).slideUp();

				});
			  });
}

function login(){
	$.ajax({
			  url: "./html/login.php",
			  cache: false
			})
			  .done(function( html ) {
			  	$( "#contenido" ).removeClass("desarrollo");
			  	$( "#contenido" ).empty();

				$( "#contenido" ).append(html);
			    if(flag_register == 0) {
			    	$( "#contenido" ).slideToggle();
			    }
			    else{
					$( "#contenido" ).slideDown();
					flag_register = 0;
			    }
			    $(".usersOptions").on("click",function(event)
					{
						sessionStorage.setItem("user",$(this).text());
						$(".usertext").text(sessionStorage.getItem("user"));
						$(".log-logout").text("log-out");
						$("#textbar").text("Start train");
						$("#textbar").closest("#barra").removeClass("choose-property").removeClass("customSelect").addClass("choose-property2").addClass("customSelect2");
						$( "#contenido" ).empty();
						$( "#contenido" ).slideUp();
						$(".register").hide();
					});

			    $(".register").on("click",function()
					{
						if(sessionStorage.getItem("user") == null){
							regist();
						}
						else{
							alert("Please log-out first.");
						}
					});
			  });

	
}

$(document).ready(function(){
	$(".usertext").text(sessionStorage.getItem("user"));
	if(sessionStorage.getItem("user") != null)
	{
		$(".log-logout").text("log-out");
		$("#textbar").text("Start train");
		$("#textbar").closest("#barra").removeClass("choose-property").removeClass("customSelect").addClass("choose-property2").addClass("customSelect2");
		$(".register").hide();
	}
	else
	{
		$("#textbar").text("Log in");
		$(".log-logout").text("log-in");
		$("#textbar").closest("#barra").removeClass("choose-property2").removeClass("customSelect2").addClass("choose-property").addClass("customSelect");
	}
	
	$(".mainTaskBar").on("click",function()
	{
		if( $(".log-logout").text() == "log-out" ){
			$("#option-boxes").slideToggle();
			$("#contenido").slideToggle();
			$("#exec").slideToggle();
		}
		else{
			login();
		}
	});

	$(".log-logout").on("click",function()
	{
		if( $(".log-logout").text() == "log-out")
		{
			sessionStorage.removeItem("user");
			$(".log-logout").text("log-in");
			$("#textbar").text("Log in");
			$("#option-boxes").slideUp();
			$(".usertext").text(sessionStorage.getItem("user"));
			$("#textbar").closest("#barra").removeClass("choose-property2").removeClass("customSelect2").addClass("choose-property").addClass("customSelect");
			$(".register").show();
			$(".option-right").empty();
			$(".option-right").removeClass("genome-desarrollo");
			$("#contenido").empty();
			$("#contenido").removeClass("desarrollo");
			$("#exec").slideUp();
		}
		else
		{
			login();
		}
	});
	
	$(".register").on("click",function()
	{
		if(sessionStorage.getItem("user") == null){
			regist();
		}
		else{
			alert("Please log-out first.");
		}
		
	});

	$("#choose-genome").on("click",function(){
		$.ajax({
			  url: "./html/genome.php",
			  cache: false
			})
			  .done(function( html ) {
			  	$(".option-right").empty();
			  	$(".option-right").append(html);
			  	$(".option-right").addClass("genome-desarrollo");
			  	$("#contenido").empty();
			  	$("#contenido").removeClass("desarrollo");
			  });
	});

	$("#choose-probabilities").on("click",function(){
		$.ajax({
			  url: "./html/probabilities.php",
			  cache: false
			})
			  .done(function( html ) {
			  	$(".option-right").empty();
			  	$("#contenido").empty();
			  	$("#contenido").removeClass("desarrollo");
			  	$(".option-right").append(html);
			  	$(".option-right").addClass("genome-desarrollo");

			  	$("#button-probabilities").on("click",function(){
			  		$.ajax({
						  url: "./html/button_probabilities.php",
						  cache: false
						})
						  .done(function( html ) {
			  				//nada
			  				alert("nada");
			  			});
			  		
			  	});

			  });
	});

	$("#choose-fitness").on("click",function(){
		$.ajax({
			  url: "./html/fitness.php",
			  cache: false
			})
			  .done(function( html ) {
			  	$(".option-right").empty();
			  	$(".option-right").removeClass("genome-desarrollo");
			  	$("#contenido").empty();
			  	$("#contenido").addClass("desarrollo");
			  	$("#contenido").append(html);
			  	$( "#contenido" ).fadeIn();
			  });
	});

	$("#compile").on("click",function(){
		$.ajax({
			  url: "./html/compile.php",
			  cache: false
			})
			  .done(function( html ) {
			  	$(".option-right").empty();
			  	$("#contenido").empty();
			  	$("#contenido").removeClass("desarrollo");
			  	$(".option-right").append(html);
			  	$(".option-right").addClass("genome-desarrollo");
			  });
	});

	$("#run").on("click",function(){
		$.ajax({
			  url: "./html/run.php",
			  cache: false
			})
			  .done(function( html ) {
			  	$(".option-right").empty();
			  	$("#contenido").empty();
			  	$("#contenido").removeClass("desarrollo");
			  	$(".option-right").append(html);
			  	$(".option-right").addClass("genome-desarrollo");
			  });
	});

});

