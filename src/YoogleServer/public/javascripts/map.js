var map;
var current_idx = -1;
var locationMarker;

function markerClick(idx) {
	if(current_idx != -1) {
		var old_marker = marker_list[current_idx];
		old_marker.setMap(null);
		marker_list[current_idx] = new google.maps.Marker({
			position : old_marker.getPosition(),
			map : map,
			title : old_marker.getTitle(),
			icon : "images/mark_"+current_idx+".png"
		});
	}
	var marker = marker_list[idx];
	marker.setMap(null);
	marker_list[idx] = new google.maps.Marker({
		position : marker.getPosition(),
		map : map,
		title : marker.getTitle(),
		icon : "images/mark_"+idx+".png",
		animation : google.maps.Animation.BOUNCE
	});
	map.panTo(marker.getPosition());
	current_idx = idx;
}

function initializeMap() {
	var latlng = new google.maps.LatLng(1.3, 103.6);
	var myOptions = {
		zoom : 12,
		center : latlng,
		mapTypeId : google.maps.MapTypeId.ROADMAP
	};
	map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);

	locationMarker = new google.maps.Marker({
		position : latlng,
		map : map,
		title : "I am here!",
		icon : "http://maps.google.com/mapfiles/marker_grey.png",
		draggable : true
	});
	map.panTo(locationMarker.getPosition());
	google.maps.event.addListener(locationMarker, 'click', function() {
		map.panTo(locationMarker.getPosition());
	});
	google.maps.event.addListener(locationMarker, 'dragend', function(pos) {
		var pos_data = {
			lat_coord : pos.latLng.lat(),
			lng_coord : pos.latLng.lng()
		};
		$("#lat_text").html(pos.latLng.lat());
		$("#lng_text").html(pos.latLng.lng());
		$.ajax({
			url : 'set_position',
			data : pos_data,
			type : "POST",
			dataType : 'json',
			success : function(json) {
				keypress(true);
			},
			error : function(err) {
				alert("no post data");
			}
		});
	});
}

function focusCenter() {
	map.panTo(locationMarker.getPosition());
}

function keypress(force) {
						if(!force && input_string == $("#querytext").val())
							return;
						input_string = $("#querytext").val();
						//keypress();
						$.ajax({
							url : 'list_search',
							data : { query : input_string },
							type : "POST",
							dataType : 'json',
							success : function(json) {
								//alert(json['time']);
								//$("li").remove();
								//alert("time:"+json.time+" length:"+res_list.length);
								$("#query").find("li").remove();
								for(var i = 0; i < marker_count; ++i) {
									marker_list[i].setMap(null);
								}
								//$("#query").html("i");
								//document.getElementById("query").innerHTML="";
								
								$(".InputDiv").find("span").remove();
								$(".InputDiv").append("<span class='TimeText'>Elapsed Time : "+json.time+" s</span>");
								var res_list = json.result;
								marker_count = res_list.length;
								for(var i = 0; i < res_list.length; ++i) {
									var litem = "<li class='ResultItem' onclick=\"markerClick("+i+")\">";
									litem += "<div class='ResultItemMarker'><img src='images/mark_"+i+".png'/></div>";
									litem += "<div class='ResultItemContent'>";
									//$("#query").append("<li class='ResultItem'>"+res_list[i].name+"</li>");
									litem += "<a class='ResultItemName' onclick=\"markerClick("+i+")\">"+res_list[i].name+"</a><br>";
									litem += res_list[i].addr.replace(/\n/g, "<br>")+"<br>";
									litem += res_list[i].post_code+"  · "+"<a class='ResultItemHref' href="+res_list[i].url+">"+res_list[i].url+"</a>";
									litem += "</div></li>";
									$("#query").append(litem);

									var latlng = new google.maps.LatLng(res_list[i].lat_coord, res_list[i].lng_coord);
									marker_list[i] = new google.maps.Marker({
										position : latlng,
										map : map,
										title : res_list[i].name,
										icon : "images/mark_"+i+".png"
									});
								}
								current_idx = -1;
							},
							error : function(err) {
								alert('error!!!');
							}
						});
}
