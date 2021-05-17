// send data into server, data must be dictionary
var a;

function getJSON(page=""){
    var ret = "";
    var myArr;
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            //console.log(this.responseText);
            try {
                myArr = JSON.parse(this.responseText);
            } catch (e){
				console.log(this.responseText);
                console.log("Json Data => null");
                myArr = {};
            }
        }
    };
    if(page == ""){
        xmlhttp.open("POST", "actions.php", false);
    }else{
        xmlhttp.open("POST", String(page), false);
    }
    xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xmlhttp.send();
	//console.log(myArr);
	a = myArr;
    return(myArr);
}

function sendGET(page){
	var ret;
    var xmlhttp = new XMLHttpRequest();
	xmlhttp.onreadystatechange = function(){
		if(xmlhttp.readyState == 4 && xmlhttp.status == 200){
			ret = xmlhttp.responseText;
		}
	}
    xmlhttp.open("GET", String(page), true);
    xmlhttp.send();
	return ret;
}


function getSizeOfScrean(){
	var win = window, doc = document, docElem = doc.documentElement,
	body = doc.getElementsByTagName('body')[0],
    x = win.innerWidth || docElem.clientWidth || body.clientWidth,
    y = win.innerHeight|| docElem.clientHeight|| body.clientHeight;
	console.log(x + ' × ' + y);
	var ret = {};
	ret["x"] = x;
	ret["y"] = y;
	return ret;
}

function createGIVStatus(jsonData){
	var screen = getSizeOfScrean();
	var x = Math.round(screen['x']*0.5);
	if(x < 360){
		x = 360;
	}
	var xx = Math.round(x/4);
	var y = 2*xx;
	var m = Math.round(xx*1.5);
	var r = Math.round(xx/3);
	var ret = "";
	ret += "<svg  width='" + x + "' height='" + y + "' version='1.1'>";
	ret += "<circle cx='" + xx + "' cy='" + xx + "' r='"+r+"' stroke='#5cb85c' stroke-width='6' fill='" + jsonData['color1'] + "'></circle>";
	ret += "<circle cx='" + 2*xx + "' cy='" + xx + "' r='"+r+"' stroke='orange' stroke-width='6' fill='" + jsonData['color2'] + "'></circle>";
	ret += "<circle cx='" + 3*xx + "' cy='" + xx + "' r='"+r+"' stroke='red' stroke-width='6' fill='" + jsonData['color3'] + "'></circle>";
	ret += "<text x='" + xx + "' y='" + m + "' text-anchor='middle'  fill='#5cb85c'>" + jsonData['text1'] + "</text>";
	ret += "<text x='" + 2*xx + "' y='" + m + "' text-anchor='middle'  fill='orange'>" + jsonData['text2'] + "</text>";
	ret += "<text x='" + 3*xx + "' y='" + m + "' text-anchor='middle'  fill='red'>" + jsonData['text3'] + "</text>";
	ret += "</svg>";
	return ret;
}

function indexLoop(){

	document.getElementById("divSVG").innerHTML = createGIVStatus(getJSON("/getLedData")[0]);

	var data = getJSON("/getTemperatureData")
	if(isNaN(data[0]["temperature"])){
		document.getElementById("temperature").innerHTML = String(data[0]["temperature"]);
	}else{
		document.getElementById("temperature").innerHTML = String(data[0]["temperature"]) + "°C";
	}
	document.getElementById("setedtemperature").innerHTML = String(data[0]["setedtemperature"]);
	document.getElementById("setTemp").value = data[0]["setedtemperature"];
	document.getElementById("hysterez").innerHTML = String(data[0]["hysterez"]);
	document.getElementById("setHys").value = data[0]["hysterez"];
	console.log(data[0]["active"]);
	if(Number(data[0]["active"]) == 1){
		document.getElementById("switch").checked = true;
	}else{
		document.getElementById("switch").checked = false;
	}
}
