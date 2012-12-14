function initializeMap() {
	var latlng = new google.maps.LatLng(39,116);
	var myOptions = {
		zoom : 12,
		center : latlng,
		mapTypeId : google.maps.MapTypeId.ROADMAP
	};
	var map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);

	var mymarker = new google.maps.Marker({
		position : latlng,
		map : map,
		title : "I am here!",
		icon : "http://maps.google.com/mapfiles/marker_grey.png"
	});
	map.setCenter(mymarker.getPosition());
	google.maps.event.addListener(mymarker, 'click', function() {
		map.setCenter(mymarker.getPosition());
	});
}

function keypress() {
	var query = document.getElementById("querytext").value;
	document.getElementById("showquery").innerHTML = query;
	document.getElementById("showqueryext").innerHTML = query;
	if(query.length == 0) {
		return;
	}
	//TODO
}

