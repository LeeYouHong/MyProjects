<?php
    class testController {
        function show() {
	    global $view;
            $testModel = M('test');
	    $data = $testModel->get();

	    //$testView = V('test');
	    //$testView->display($data);
	    $view->assign('str', '������');
            $view->display('test.tpl');
	}
    }
?>
