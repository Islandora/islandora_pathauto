islandora_pathauto
==================

Exposes Islandora objects to the alias-creating tools of pathauto. 


Requirements
------------

* [Pathauto](https://www.drupal.org/project/pathauto)
* [Token](https://www.drupal.org/project/token)
* [Islandora](https://github.com/Islandora/islandora)

Configuration
-------------

To enable pathauto for a content model visit Administration >> Islandora >> Islandora Utility 
Moduls >> Pathauto (admin/islandora/tools/islandora-pathauto). To configure the aliases that pathauto
will create, visit Administration >> Configuration >> Search and Metadata >> URL Aliases >> Patterns 
(admin/config/search/path/patterns). You can create aliases including the object's pid ([fedora:pid]), 
the Fedora label ([fedora:label]), and/or the pid without the namespace ([fedora:shortpid]). See the documentation
for [Pathauto](https://www.drupal.org/documentation/modules/pathauto) for more information on creating aliases.

Suggested Implementation
------------------------

Without [Sub-pathauto](https://www.drupal.org/project/subpathauto), islandora objects will be viewable at configured aliases,
but the datastreams (i.e. [OBJECT] + /datastream/DSID/view) will only be accessible at the original URL, islandora/object/PID/datastream/DSID/view. If you want 
datastreams to be visible at OBJECT-ALIAS/datastream/DSID/view, then enable Sub-pathauto and set the  maximum depth 
of sub-paths to at least 3.

When Drupal creates a link to an aliased object, it will direct the user to the pretty URL. But if the user navigates directly to islandora/object/PID (i.e. types it in
the address bar), they will also get the object page. If you want the original islandora URLs to resolve (i.e. redirect) to the aliases, 
then enable [Global Redirect](https://www.drupal.org/project/globalredirect). 

By default, Pathauto removes punctuation such as the colon (:) from paths before creating aliases. This will result
in PIDs that look like islandora123; if this is undesirable then there's a setting under "punctuation" for Pathauto 
at admin/config/search/path/settings to not remove the colon.
 

Edge Cases
----------

If you have an object with multiple pathauto-enabled content models, then two aliases for that object will be created.
This may cause unpredictable behaviour if you use Global Redirect to force a single, "canonical" alias. 

