islandora_pathauto
==================

Exposes islandora objects to the alias-creating tools of pathauto

Please feel free to use this as a proof-of-concept, but there is a lot of local configuration going on.
The "clean" branch should be closer to something useable on a stock Islandora installation.


IMPORTANT!!!
============

There is a hard-coded SPARQL query that'll mess you up because you probably don't have a collection called pubs:collection. 
Change it to something that'll select the objects you want to alias. It's in islandora_pathauto_pathauto_bulkupdate(). 

Configuration
-------------

In order to access [object]/datastream/MODS/view, etcetera, you need to have sub-pathauto enabled, and aliasing subpaths to a depth of at least 3. 
