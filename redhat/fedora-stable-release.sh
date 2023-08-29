#!/bin/bash

releasenum=$1

if [ -z "$releasenum" ]; then
	releasenum="0"
fi

klist -s
if [ ! $? -eq 0 ]; then
	echo "klist couldn't read the credential cache."
	echo "Do you need to fix your kerberos tokens?"
	exit 1
fi

ApplyPatches="0"

for release in $( cat redhat/release_targets );  do 
	case "$release" in
	39) build=30$releasenum
	    ;;
	38) build=20$releasenum
	    ;;
	37) build=10$releasenum
	    ;;
	esac
	if [[ $ApplyPatches == "1" ]] ; then
		for patch in redhat/patches/* ; do patch -p1 < $patch ; done
	fi
	make IS_FEDORA=1 DIST=".fc$release" BUILDID="" BUILD=$build RHDISTGIT_BRANCH=f$release dist-git;
	sleep 60;
	git checkout .
done
