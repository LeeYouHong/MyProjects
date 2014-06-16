<?php
    // Controller
    function C($name, $method) {
        require_once('libs/Controller/'.$name.'.class.php');
	$controller = $name.'controller';
	$obj = new $controller;
	$obj->$method();
    }

    // Model
    function M($name) {
        require_once('libs/Model/'.$name.'.class.php');
	$model = $name.'Model';
	$obj = new $model;
	return $obj;
    }

    // View
    function V($name) {
        require_once('libs/View/'.$name.'.class.php');
	$view = $name.'View';
	$obj = new $view;
	return $obj;
    }

    function daddslashes($str) {
        return (!get_magic_quotes_gpc())?addslashes($str):$str;
    }

    function ORG($path, $name, $params=array()) {
        require_once('libs/ORG/'.$path.$name.'.class.php');
	$obj = new $name();
	if (!empty($params)) {
            foreach($params as $key=>$value) {
		$obj->$key = $value;
	    }
	}
	return $obj;
    }
?>
