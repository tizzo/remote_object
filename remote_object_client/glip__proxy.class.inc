<?php

/**
 * @file
 *
 * Define a set of proxy objects to in place of the native glip object
 * so that all glip operations can be performed on a remote machine separate
 * from the one where the public facing Drupal instance is running.
 */

class Git__proxy extends remoteProxyClass {}

class Binary__proxy extends remoteProxyClass {}

class GitObject__proxy extends remoteProxyClass {}

class GitBlob__proxy extends remoteProxyClass {}

class GitCommit__proxy extends remoteProxyClass {}

class GitCommitStamp__proxy extends remoteProxyClass {}

class GitTree__proxy extends remoteProxyClass {}

