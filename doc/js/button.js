function endsWith(str,ending) {
	return str.indexOf(ending,str.length-ending.length)!==-1;
}
function contains(str,part) {
	return str.includes(part);
}

function setActive(btn,img) {
	document.getElementById("button-"+btn).
			style.backgroundImage="url(images/"+img+")";
}

function setButton() {
	if (endsWith(document.location.pathname,"index.html") ||
			endsWith(document.location.pathname,"/")) {
		setActive("about","lightbluedenim.png");
	} else if (endsWith(document.location.pathname,"documentation.html") ||
			contains(document.location.pathname,"/programming/")) {
		setActive("documentation","lightbluedenim.png");
	} else if (endsWith(document.location.pathname,"download.html")) {
		setActive("download","lightbluedenim.png");
	} else if (endsWith(document.location.pathname,"license.html")) {
		setActive("license","lightbluedenim.png");
	}
}
