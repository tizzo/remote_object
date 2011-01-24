<?php

/**
 *
 */

/**
 *
 */
class remoteProxyClass {

  // The actual object from the remote machine.
  public $remoteObject = null;

  // The class of the remote object.
  public $remoteClass = '';

  // The Drupal site you wish to use as the remote for your this object.
  // Note: For security this should always be an SSL (https) connection.
  // TODO: if this ever moves beyond a proof of concept, use services?
  public $remoteURL = '';

  // The username of the user to act as on the remote machine.
  public $remoteUsername = '';

  // The password of the user to act as on the remote machine.
  public $remotePassword = '';

  /**
   * Generic constructor that will 
   *
   * @params (mixed)
   *    Any params used will be passed to the contructor on the remote machine.
   *
   * @return (mixed)
   *   The fully loaded object from the remote machine.
   */
  public function __construct() {

    $class = get_class($this);
    // Verify that this is a proper proxy object and then store the class name if so.
    if (strpos($class, '__proxy') !== FALSE) {
      $this->remoteClass = substr($class, 0, -7);
    }
    else {
      // If this object was not named properly, die screaming.
      throw new Exception('proxyClass descended objects must have a class name ending in __proxy');
    }
    $args = func_get_args();
    if ($credentials = array_shift($args)) {
      $this->proxy__configure__proxy($credentials);
    }
    $data = new stdClass;
    $data->class = $this->remoteClass;
    $data->args = array_shift($args);
    $this->getRemoteObject($data);
  }

  /**
   * This function is used to populate data
   */
  public function proxy__configure__proxy($data) {
    foreach ($data as $name => $value) {
      $this->$name = $value;
    }
  }

  /**
   * Reroute all method calls to an object on a remote system.
   *
   * All method calls against classes descented from this one should be
   * unimplemented and therefore should be sent to __call.
   */
  public function __call($name, $arguments) {
    $data = new stdClass;
    $data->class = $this->remoteClass;
    $data->method = $name;
    $data->arguments = $arguments;
    $data->object = $this->remoteObject;
    return $this->remoteExecute($data);
  }
 
  /**
   * Implementing static method calls across a remote proxy connection are currently unsupported.
   */
  public function __callStatic($name, $arguments) {
    throw new Exception('Static methods are not available on remote objects using proxyClass');
  }

  // TODO: test
  public function __set($name, $value) {
    $this->remoteObject->$name = $value;
  }

  // TODO: test
  public function __get($name) {
    return $this->remoteObject->$name;
  }

  // TODO: Figure out what to do with isset.
  public function __isset($name) {
    return isset($this->remoteObject->$name);
  }

  // TODO: Figure out what to do with unset.
  public function __unset($name) {
    unset($this->remoteObject->$name);
  }

  /**
   * Retrieve a remote object.
   */
  public function getRemoteObject($data) {
    $data->method = 'proxy__proxyGetObject__proxy';
    $post_data['username'] = $this->remoteUsername; 
    $post_data['password'] = $this->remotePassword; 
    $post_data['remote_object_data'] = serialize($data);
    $data = drupal_http_request($this->remoteURL, array('Content-Type' => 'application/x-www-form-urlencoded'), 'POST', http_build_query($post_data));
    $remoteObject = unserialize(base64_decode($data->data));
    $this->remoteObject = $remoteObject;
  }

  /**
   * Retrieve contents from 
   */
  public function remoteExecute(stdClass $object_data) {
    // Populate authentication information
    $post_data['username'] = $this->remoteUsername; 
    $post_data['password'] = $this->remotePassword; 
    $post_data['remote_object_data'] = serialize($object_data);
    $response_data = drupal_http_request($this->remoteURL, array('Content-Type' => 'application/x-www-form-urlencoded'), 'POST', http_build_query($post_data));
    if (!$data = unserialize(base64_decode($response_data->data))) {
      throw new Exception('Invalid data received from remote.  Remote Object Client just blew up.  Sorry, but we all knew this would happen.');
    }
    $return = $data->result;
    // If the returned value is an object, we need to proxify it so that any subsequent calls on the returned object are run on the remote system.
    if (is_object($return) && class_exists($proxy_class = get_class($return) . '__proxy')) {
      $args = array(
        'remoteURL' => $this->remoteURL,
        'remoteUsername' => $this->remoteUsername,
        'remotePassword' => $this->remotePassword,
        'remoteObject' => $return,
      );
      $object = new $proxy_class;
      $object->proxy__configure__proxy($args);
      return $object;
    }
    return $return;
  }
}
