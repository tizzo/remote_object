<?php

/**
 * @file
 */

class remoteObjectServer {

  public function getObject($data) {
    $reflectionObject = new ReflectionClass($data->class);
    $object = $reflectionObject->newInstanceArgs($data->args);
    return $object;
  }

  public function remoteExecute($data) {
    if ($data->method == 'proxy__proxyGetObject__proxy') {
      return $this->getObject($data);
    }
    elseif (is_object($data->object)) {
      $result =  call_user_func_array(array(&$data->object, $data->method), $data->arguments);
    }
    else {
      $result = call_user_func_array(array($data->class, $data->method), $data->arguments);
    }
    $return = new stdClass;
    $return->object = $data->object;
    $return->result = $result;
    return $return;
  }

}


