
/*
 * POST home page.
 */

var ydb = require('ydb');

exports.list_search = function(req, res){
	/*
	var test = {
		title : 'Express'
	};
	*/
	console.log(req.body.query);
	var data = ydb.prefix_search(req.body.query);
	res.send(data);
	console.log(data);
};

exports.set_position = function(req, res) {
	ydb.set_position(req.body.lat_coord, req.body.lng_coord); 
	res.send({ result : 0 });
};
