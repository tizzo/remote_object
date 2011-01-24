## _THIS MODULE IS INSECURE!_ ##

It is merely an experiment and is inherently highly dangerous.  The module executes code on a remote server over a web service and currently provides no (working) authentication 

## Remote Object ##

Remote Object is a pair of Drupal modules built from the ground up to visit evil upon the world and eat all of your children.

Well, actually that's just a likely side effect.  This module was written as an experiment to see how we could get [glip](https://github.com/patrikf/glip) operations to run on a remote machine with as little code as possible.  It is a horrible PHP hack created as an experiment and shared here because ...well... why not?


Remote object provides two modules, a client and a server.  The client provides a factory function which can be used to retrieve php objects from the remote server.  Any method executed against the retrieved object is then routed back to the remote site, executed there, and the result is returned (and wrapped in another proxy object if necessary).

## Under the hood ##

When the client module's factory method is used to retrieve an object from a remote site a web service is called against the configured remote (currently configuration for your object type must be done in settings.php but we'll need to change that) where the object is instantialiated, base64 encoded and returned.  The returned object is then placed inside the proxy object and when any method called on the proxy, the proxy intercepts the method call via the `__call` magic method,serializes and base64 encodes the remote object, sends the object to the web service on the remote where the object is decoded, the method is run with whatever arguments were used, and the result is returned.

## Usage ##

Don't.  It was written as a proof of concept and never intended to actual use.
