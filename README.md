Please follow these instructions to run the program.

Execute these command in the given order:

1> Start the mongodb server by running command 'mongodb --dbpath data/'. You need to be inside solar-web folder ('cd solar-web/').

2> Start the node server by running command 'npm start'.

3> From 'src' folder type the command 'ng serve' to start the angular client.

4> Return to the root folder (dissertation code files) and start the python service by running command 'python mqtt_py_script.py'.

5> Open web application from browser at url: http://localhost:4200

6> Run the random data generator by running command 'python dataGen.py'

If everything runs without any error then you should see the graph getting updated.


Troubleshooting dependecy issues:

> npm install <dependency name> --save