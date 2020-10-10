document.cookie = "username=dev";
	
function myfunc() {
	var a = document.getElementById('first').value;
	var b = document.getElementById('second').value;

	document.getElementById('plus').addEventListener("click", function() {
		var c = a+b;
		document.getElementById('result').innerHTML = c;
		let data = {
			value: c
		}
		$.post("/page1",data)
			.done(function(result) {
				//error1
			})
	})

	document.getElementById('minus').addEventListener("click", function() {
		var c = a-b;
		document.getElementById('result').innerHTML = c;
		let data = {
			value: c
		}
		$.post("/page2",data)
			.done(function(result) {
				//error2
			})
	})

	document.getElementById('multiply').addEventListener("click", function() {
		var c = a*b;
		document.getElementById('result').innerHTML = c;
		let data = {
			value: c
		}
		$.post("/page3",data)
			.done(function(result) {
				//error3
			})
	})

	document.getElementById('divide').addEventListener("click", function() {
		var c = a/b;
		document.getElementById('result').innerHTML = c;
		let data = {
			value: c
		}
		$.post("/page4",data)
			.done(function(result) {
				//error4
			})
	})	