FROM nvidia/cuda:8.0-devel-ubuntu16.04
MAINTAINER Andrew Seidl <dev@aas.io>

ADD gl-version-test /gl-version-test

CMD /gl-version-test/gl-version-test
