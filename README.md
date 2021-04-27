# Motion-Detector
Ooh look, some bad code fresh from the oven. It work with varing degrees of success depending on the lighting. You'll have to mess with the commented values. You need the escapi binaries you can download them here: http://iki.fi/sol/zip/escapi3.zip and here is the github repo: https://github.com/jarikomppa/escapi.
## What does it do?
It continuously captures images using escapi and stores them in a array. The arrays are then filtered to get rid of some unwanted movements. The filtered array is then compared with the previous image's filtered array. If enough movement has taken place it goes to the tab switcher using Windows.h.
