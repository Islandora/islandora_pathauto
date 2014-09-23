islandora_pathauto
==================

Exposes Islandora objects to the alias-creating tools of pathauto. 

Configure alias patterns in the pathauto config interface, at admin/config/search/path/patterns.  


Requirements
------------

* [Pathauto](https://www.drupal.org/project/pathauto)
* [Token](https://www.drupal.org/project/token)
* [Islandora](https://github.com/Islandora/islandora)


Suggested Implementation
------------------------

Without [Sub-pathauto](https://www.drupal.org/project/subpathauto), islandora objects will be viewable at configured aliases,
but the /datastream/DSID/view paths will still be at islandora/object/PID/datastream/DSID/view. If you want 
datastreams to be visible at OBJECT-ALIAS/datastream/DSID/view, then enable Sub-pathauto and set the  maximum depth 
of sub-paths to at least 3.

Drupal aliases provide nice URLs but leave the old URLs still accessible. If you want islandora/object/PID to 
redirect users to the alias, then enable [Global Redirect](https://www.drupal.org/project/globalredirect). 

By default, Pathauto removes punctuation such as the colon (:) from paths before creating aliases. This will result
in PIDs that look like islandora123; if this is undesirable then there's a setting under "punctuation" for Pathauto 
at admin/config/search/path/settings that lets you keep the colon.
 
Known Issues
------------

If your alias pattern includes the object's label, then modifying the object's label will not immediately modify the alias. 
When the object is modified repeatedly, then the alias will always be one version behind. This issue is documented
at https://jira.duraspace.org/browse/ISLANDORA-1073.

