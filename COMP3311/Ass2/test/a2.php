<?php

// If you want to use the COMP3311 DB Access Library, include the following two lines
//
define("LIB_DIR","/import/adams/1/cs3311/public_html/19s1/assignments/a2");
require_once(LIB_DIR."/db.php");

// Your DB connection parameters, e.g., database name
//
define("DB_CONNECTION","dbname=a2");

//
// Include your other common PHP code below
// E.g., common constants, functions, etc.
//

function printmovieinfo($db, $res, $limit, $startyear, $endyear) {
	$i = 1;
	while ($movie = dbNext($res)) {
	$q = "select m.title, m.year, m.content_rating, m.lang, r.imdb_score, r.num_voted_users from movie m, rating r where r.movie_id = m.id and m.title = %s and m.year >= %d and m.year <= %d order by r.imdb_score DESC, r.num_voted_users DESC limit %d";
	$r = dbQuery($db, mkSQL($q, $movie[0], $startyear, $endyear, $limit));

// Iterate through the results and print
	while ($t = dbNext($r)) {
		echo "$i. ";
		if (!empty($t[0]))
		echo $t[0]; 
		echo " (";
		if (!empty($t[1]))
		echo $t[1].", ";
		if (!empty($t[2]))
		echo "$t[2], ";
		if (!empty($t[3]))
		echo "$t[3]";
		echo ")";
		
		echo " [";
		if (!empty($t[4]))
		echo "$t[4]";
		if (!empty($t[5]))
		echo ", $t[5]";
		echo "]\n";	
		$i++;
	}
	}
}

function addquotes($array, $n) {
	// convert the array of strings into something we can put in the sql query
	$i = 0;
	while ($i < $n) {
		$array[$i] = "'".$array[$i]."'";
		$i++;
	}
	$newlist = join(", ", $array);
	return $newlist;
}

function search($db, $node, $goal, $degree, $parent_node, $visited, $max_degree) {
	$visited[$node['actor']] = true;
	if ($node['actor'] == $goal) {
		$path_list = array();
		$path = array();
		array_push($path, $node);
		array_push($path_list, $path);
		return $path_list;
	}
	if ($degree > ($max_degree-1)) {
		$path = array();
		return $path;
	}
	$t_list = array();

	$list = getneighbours($db, $node['actor']);
	foreach ($list as $neighbour) {
		if (!isset($visited[$neighbour['actor2']])) { //node not already visited
			$tnode = array();
			$tnode['actor'] = $neighbour['actor2'];
			$tnode['movie'] = $neighbour['movie_id'];
			$paths = search($db, $tnode, $goal, $degree + 1, $node['actor'], $visited, $max_degree); 
			foreach ($paths as $x) {
				array_push($x, $tnode);
				array_push($t_list, $x);
			}
		}
	}
	return $t_list;
}

function getneighbours($db, $actor_id) {
	if (!isset($GLOBALS['m'][$actor_id])){
		$q = "select * from neighbours where actor_id1 = %d";
		$r = dbQuery($db, mkSQL($q, $actor_id));
		$tarray = array();
		while ($curr = dbNext($r)) {
			$temp = array();
			$temp['actor1'] = $curr['actor_id1'];
			$temp['actor2'] = $curr['actor_id2'];
			$temp['movie_id'] = $curr['movie_id'];
			array_push($tarray, $temp);
		}
		return $tarray;
	} 
	return $GLOBALS['m'][$actor_id];
}

?>
