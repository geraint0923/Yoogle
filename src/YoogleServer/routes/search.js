
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
	var data = ydb.prefix_search(req.body.lat_coord, req.body.lng_coord, req.body.query);
	res.send(data);
	console.log(data);
};

