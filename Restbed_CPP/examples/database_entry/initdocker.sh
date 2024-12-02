    #!/bin/sh

    cp -R ../../restbed .
    docker build --no-cache -t $1 .
    docker run -dp $2 $1
    rm -R restbed
