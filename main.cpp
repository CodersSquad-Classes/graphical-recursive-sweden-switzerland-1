#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
using namespace std;

struct Point {
    double x, y;
};

void writeLine(ofstream &linesFile, Point p1, Point p2) {
    linesFile << "(" << fixed << setprecision(2) << p1.x << "," << p1.y << ")"
              << "(" << p2.x << "," << p2.y << ")" << endl;
}

void kochCurve(ofstream &linesFile, Point p1, Point p5, int levels) {
    if (levels == 0) {
        writeLine(linesFile, p1, p5);
        return;
    }

    Point p2, p3, p4;
    double dx = (p5.x - p1.x) / 3.0;
    double dy = (p5.y - p1.y) / 3.0;

    p2.x = p1.x + dx;
    p2.y = p1.y + dy;

    p4.x = p1.x + 2 * dx;
    p4.y = p1.y + 2 * dy;

    // vector from p2 to p4
    double vx = p4.x - p2.x;
    double vy = p4.y - p2.y;

    // rotate vector by +60°
    double angle = M_PI / 3.0;
    p3.x = p2.x + (vx * cos(angle) - vy * sin(angle));
    p3.y = p2.y + (vx * sin(angle) + vy * cos(angle));

    kochCurve(linesFile, p1, p2, levels - 1);
    kochCurve(linesFile, p2, p3, levels - 1);
    kochCurve(linesFile, p3, p4, levels - 1);
    kochCurve(linesFile, p4, p5, levels - 1);
}

void hilbert(ofstream &linesFile, double x, double y, double xi, double xj, double yi, double yj, int n) {
    if (n <= 0) {
        Point p1{ x + (xi + yi) / 2, y + (xj + yj) / 2 };
        static Point prev = p1;
        static bool first = true;
        if (!first) {
            writeLine(linesFile, prev, p1);
        }
        prev = p1;
        first = false;
    } else {
        hilbert(linesFile, x, y, yi/2, yj/2, xi/2, xj/2, n-1);
        hilbert(linesFile, x+xi/2, y+xj/2, xi/2, xj/2, yi/2, yj/2, n-1);
        hilbert(linesFile, x+xi/2+yi/2, y+xj/2+yj/2, xi/2, xj/2, yi/2, yj/2, n-1);
        hilbert(linesFile, x+xi/2+yi, y+xj/2+yj, -yi/2, -yj/2, -xi/2, -xj/2, n-1);
    }
}

void hilbertCurve(ofstream &linesFile, Point a, Point b, int levels) {
    // set size to span between a and b (assume horizontal line)
    double size = fabs(b.x - a.x);
    hilbert(linesFile, a.x, a.y, size, 0, 0, size, levels);
}

void sierpinski(ofstream &linesFile, Point a, Point b, Point c, int levels) {
    if (levels == 0) {
        writeLine(linesFile, a, b);
        writeLine(linesFile, b, c);
        writeLine(linesFile, c, a);
        return;
    }

    // midpoints
    Point ab{ (a.x+b.x)/2, (a.y+b.y)/2 };
    Point bc{ (b.x+c.x)/2, (b.y+c.y)/2 };
    Point ca{ (c.x+a.x)/2, (c.y+a.y)/2 };

    sierpinski(linesFile, a, ab, ca, levels-1);
    sierpinski(linesFile, ab, b, bc, levels-1);
    sierpinski(linesFile, ca, bc, c, levels-1);
}

void sierpinskiTriangle(ofstream &linesFile, Point a, Point b, int levels) {
    // build an equilateral triangle from a,b
    Point c{ (a.x+b.x)/2, (a.y+b.y)/2 + (sqrt(3)/2)*(b.x-a.x) };
    sierpinski(linesFile, a, b, c, levels);
}

int main(int argc, char *argv[]) {
    ofstream linesFile("lines.txt");
    
    // random starting points
    Point a{0, 0};
    Point b{300, 0};

    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " --koch|--hilbert|--sierpinski <n_levels>\n";
        return 1;
    }

    string mode = argv[1];
    int n_levels = atoi(argv[2]);
    if (n_levels < 0) {
        cerr << "n_levels must be non-negative.\n";
        return 1;
    }

    if (mode == "--koch") {
        kochCurve(linesFile, a, b, n_levels);
    }
    else if (mode == "--hilbert") {
        hilbertCurve(linesFile, a, b, n_levels);
    }
    else if (mode == "--sierpinski") {
        sierpinskiTriangle(linesFile, a, b, n_levels);
    }
    else {
        cerr << "Unknown mode: " << mode << "\n";
        return 1;
    }

    linesFile.close();
    return 0;
}
