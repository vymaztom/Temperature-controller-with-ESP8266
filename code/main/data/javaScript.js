// send data into server, data must be dictionary
var a;

var changeLanguage = 0;
var delayValue = 1000;


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

    xmlhttp.open("GET", String(page), false);

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
	if(Number(data[0]["active"]) == 1){
		document.getElementById("switch").checked = true;
	}else{
		document.getElementById("switch").checked = false;
	}
	if(changeLanguage){
		location.reload();
		changeLanguage = 0;
	}

}

function addNoteIntoWifiConnections(SSID, RSSI){
	var one = document.getElementById("WIFIDATA");

	var note = document.createElement("DIV");
	note.className = "wifiNote";

	var a = document.createElement("DIV");
	a.className = "wifiSubNote";
	var p1 = document.createElement("p");
	p1.appendChild(document.createTextNode("SSID: " + String(SSID)));
	a.appendChild(p1);
	var p2 = document.createElement("p");
	p2.appendChild(document.createTextNode("RSSI: " + String(RSSI) + " dBm"));
	a.appendChild(p2);
	a.setAttribute('style', 'width: 80%;');

	var b = document.createElement("DIV");
	b.className = "wifiSubNote";
	var img = document.createElement('img');
    img.src = "cross.png";
	img.addEventListener('click',  (event) => {
		sendGET("/WifiNets?remove=" + String(SSID));
		location.reload();
    });
	b.appendChild(img);
	b.setAttribute('style', 'width: 10%;');


	note.appendChild(a);
	note.appendChild(b);

	one.appendChild(note);

}

var NumberResults = 250;

function getGraph(){
	var data = getJSON("/ChanelNumberJSON");
	var one = document.getElementById("graph");
	if(Number(data[0]["WifiMode"]) == 0){
		document.getElementById("graphDIV").style.display = "";
		chanelNumber = Number(data[0]["ChanelNumber"]);
		var screen = getSizeOfScrean();
		one.innerHTML = "";
		var note = document.createElement("iframe");
		note.setAttribute('width', 450);
		note.setAttribute('height', 260);
		note.setAttribute('style', "border: none;");
		note.setAttribute('src', "https://thingspeak.com/channels/" + Number(chanelNumber) + "/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=" + Number(NumberResults) + "&type=line");
		one.appendChild(note);
	}else{
		one.innerHTML = "";
		document.getElementById("graphDIV").style.display = "none";
	}
}
