/*************************************************************
 * 1. Name:
 *      Ryan Madsen
 * 2. Assignment Name:
 *      Practice 02: Physics simulator
 * 3. Assignment Description:
 *      Compute how the Apollo lander will move across the screen
 * 4. What was the hardest part? Be as specific as possible.
 *      This assignment was very difficult as only some of the formulas
 *      needed were provided and we had to extrapolate a lot of
 *      the process. 
 * 5. How long did it take for you to complete the assignment?
 *      3.5 hours.
 **************************************************************/

// include "computer.hpp"
#include <iostream>  // for CIN and COUT
#include <math.h>
using namespace std;

#define WEIGHT   15103.000   // Weight in KG
#define GRAVITY     -1.625   // Vertical acceleration due to gravity, in m/s^2
#define THRUST   45000.000   // Thrust of main engine, in Newtons (kg m/s^2)

/***************************************************
 * COMPUTE DISTANCE
 * Apply inertia to compute a new position using the distance equation.
 * The equation is:
 *     s = s + v t + 1/2 a t^2
 * INPUT
 *     s : original position, in meters
 *     v : velocity, in meters/second
 *     a : acceleration, in meters/second^2
 *     t : time, in seconds
 * OUTPUT
 *     s : new position, in meters
 **************************************************/
float computeDistance(double position, double velocity, double time, double acceleration){
   return position + (velocity * time) + (.5 * acceleration * pow(time, 2));
}

/**************************************************
 * COMPUTE ACCELERATION
 * Find the acceleration given a thrust and mass.
 * This will be done using Newton's second law of motion:
 *     f = m * a
 * INPUT
 *     f : force, in Newtons (kg * m / s^2)
 *     m : mass, in kilograms
 * OUTPUT
 *     a : acceleration, in meters/second^2
 ***************************************************/
double computeAcceleration() {
    double acceleration = THRUST / WEIGHT;
    return acceleration;
}

/***********************************************
 * COMPUTE VELOCITY
 * Starting with a given velocity, find the new
 * velocity once acceleration is applied. This is
 * called the Kinematics equation. The
 * equation is:
 *     v = v + a t
 * INPUT
 *     v : velocity, in meters/second
 *     a : acceleration, in meters/second^2
 *     t : time, in seconds
 * OUTPUT
 *     v : new velocity, in meters/second
 ***********************************************/

double computeVelocity(double velocity, double acceleration, double time) {
    return velocity + acceleration * time;
}

double computeTotalVelocity(double horizontalVelocity, double verticalVelocity) {
    return sqrt(pow(horizontalVelocity, 2) + pow(verticalVelocity, 2));
}

/***********************************************
 * COMPUTE VERTICAL COMPONENT
 * Find the vertical component of a velocity or acceleration.
 * The equation is:
 *     cos(a) = y / total
 * This can be expressed graphically:
 *      x
 *    +-----
 *    |   /
 *  y |  / total
 *    |a/
 *    |/
 * INPUT
 *     a : angle, in radians
 *     total : total velocity or acceleration
 * OUTPUT
 *     y : the vertical component of the total
 ***********************************************/

double computeVerticalComponent(double totalAcceleration, double aRadians) {
    return cos(aRadians) * totalAcceleration;
}

/***********************************************
 * COMPUTE HORIZONTAL COMPONENT
 * Find the horizontal component of a velocity or acceleration.
 * The equation is:
 *     sin(a) = x / total
 * This can be expressed graphically:
 *      x
 *    +-----
 *    |   /
 *  y |  / total
 *    |a/
 *    |/
 * INPUT
 *     a : angle, in radians
 *     total : total velocity or acceleration
 * OUTPUT
 *     x : the vertical component of the total
 ***********************************************/

double computeHorizontalComponent(double totalAcceleration, double aRadians){
    return sin(aRadians) * totalAcceleration;
}

/************************************************
 * COMPUTE TOTAL COMPONENT
 * Given the horizontal and vertical components of
 * something (velocity or acceleration), determine
 * the total component. To do this, use the Pythagorean Theorem:
 *    x^2 + y^2 = t^2
 * where:
 *      x
 *    +-----
 *    |   /
 *  y |  / total
 *    | /
 *    |/
 * INPUT
 *    x : horizontal component
 *    y : vertical component
 * OUTPUT
 *    total : total component
 ***********************************************/

double calculateTotalComponent(double x, double y) {
    return sqrt((pow(x,2) + pow(y, 2)));
}


/*************************************************
 * RADIANS FROM DEGEES
 * Convert degrees to radians:
 *     radians / 2pi = degrees / 360
 * INPUT
 *     d : degrees from 0 to 360
 * OUTPUT
 *     r : radians from 0 to 2pi
 **************************************************/

double degreesToRadians(double degrees) {
    return (degrees / 360) * (2 * M_PI);
}

/**************************************************
 * PROMPT
 * A generic function to prompt the user for a double
 * INPUT
 *      message : the message to display to the user
 * OUTPUT
 *      response : the user's response
 ***************************************************/

double prompt(string message) {
    double input;
    
    cout << message;
    cin >> input;
    
    return input;
}

/****************************************************************
 * MAIN
 * Prompt for input, compute new position, and display output
 ****************************************************************/
int main()
{
    double dx;
    double dy;
    double y;
    double x = 0;
    double aDegrees;
    double t = 1;

    for (int i = 0; i < 5; i++) {   
        // Prompt for input and variables to be computed

        if (i == 0) {
            dx = prompt("\nWhat is your horizontal velocity (m/s)? ");
            dy = prompt("What is your vertical velocity (m/s)? ");
            y = prompt("What is your altitude (m)? ");
            aDegrees = prompt("What is the angle of the LM where 0 is up (degrees)? ");
        } else {
            aDegrees = prompt("\nWhat is the new angle of the LM where 0 is up (degrees)? ");
        }


        double aRadians = degreesToRadians(aDegrees);                                   // Angle in radians
        double accelerationThrust = computeAcceleration();                              // Acceleration due to thrust
        double ddxThrust = computeHorizontalComponent(accelerationThrust, aRadians);    // Horizontal acceleration due to thrust
        double ddyThrust = computeVerticalComponent(accelerationThrust, aRadians);      // Vertical acceleration due to thrust
        double ddx = computeHorizontalComponent(accelerationThrust, aRadians);          // Total horizontal acceleration
        double ddy = computeVerticalComponent(accelerationThrust, aRadians) + GRAVITY;  // Total vertical acceleration
        double totalComponent = calculateTotalComponent(ddx, ddy);
        double v;

        cout << "\nFor the next 5 seconds with the main engine on, the position of the lander is:\n" << endl;

        // Go through the simulator five times
        for (int i = 1; i < 6; i++) {
            dx = computeVelocity(dx, ddx, t);
            dy = computeVelocity(dy, ddy, t);

            x = computeDistance(x, dx, t, ddx);
            y = computeDistance(y, dy, t, ddy); 

            v = computeTotalVelocity(dx, dy);

            cout.setf(ios::fixed | ios::showpoint);
            cout.precision(2);
            cout << i << "s - " << "x,y:(" << x << ", " << y << ")m" <<  "  dx,dy:(" << dx << ", " << dy << ")m/s" << "  speed:" << v << "m/s" << "  angle:" << aDegrees << "deg" << endl;  
        }

    }
   return 0;
}