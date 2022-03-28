#include "astar_fast.h"

// codigo dificil de entender, recomendo nao utilizar

A_star_fast::A_star_fast() {
    map = NULL;
    xs = 0;
    ys = 0;
    px = NULL;
    py = NULL;
    ps = 0;
}
A_star_fast::~A_star_fast() {
    _freemap();
    _freepnt();
}
void A_star_fast::_freemap() {
    if (map) delete[] map;
    map = NULL;
    xs = 0;
    ys = 0;
}
void A_star_fast::_freepnt() {
    if (px) delete[] px;
    px = NULL;
    if (py) delete[] py;
    py = NULL;
    ps = 0;
}
//---------------------------------------------------------------------------
void A_star_fast::resize(int _xs, int _ys) {
    if ((xs == _xs) && (ys == _ys)) return;
    _freemap();
    xs = _xs;
    ys = _ys;
    map = new unsigned int * [ys];
    for (int y = 0; y < ys; y++)
        map[y] = new unsigned int[xs];
}
//---------------------------------------------------------------------------
void A_star_fast::set(BinaryImage binaryImage) {
    int c;
    resize(binaryImage.width, binaryImage.height);

    for (int y = 0; y < binaryImage.height; y++) {
        for (int x = 0; x < binaryImage.width; x++) {
            c = A_star_space;
            if (binaryImage.get(x, y) == false)
                c = A_star_wall;
            map[y][x] = c;
        }
    }
}

cimg_library::CImg<unsigned char> A_star_fast::get()
{
    int x, y;
    unsigned int c;
    cimg_library::CImg<unsigned char> result(xs, ys, 1, 3), visu(500, 400, 1, 3, 0);;

    unsigned char black[3] = { 0, 0, 0 };
    unsigned char white[3] = { 255, 255, 255 };

    for (y = 0; y < ys; y++)
        for (x = 0; x < xs; x++)
        {
            c = map[y][x];
            if (c == A_star_wall) c = 0x00000000;
            else if (c == A_star_space) c = 0x00FFFFFF;
            else                      c = ((c >> 1) & 0x7F) + 0x00404040;

            unsigned char red = (c & 0xff0000) >> 16;
            unsigned char green = (c & 0x00ff00) >> 8;
            unsigned char blue = (c & 0x0000ff);

            unsigned char color[3] = { red, green, blue };

            result.draw_point(x, y, 0, color);
        }
    cimg_library::CImgDisplay main_disp(result, "Click a point"), draw_disp(visu, "Intensity profile");
    while (!main_disp.is_closed() && !draw_disp.is_closed()) {
        main_disp.wait();
    }
    return result;
}

std::vector<point> A_star_fast::compute(int x0, int y0, int x1, int y1) {
    int foo = 0;
    return compute(x0, y0, x1, y1, foo);
}

int distance(int x0, int y0, int x1, int y1) {
    return abs(x0 - x1 + y0 - y1);
}

std::vector<point> A_star_fast::compute(int x0, int y0, int x1, int y1, int &nodes)
{
    int x, y, xmin, xmax, ymin, ymax, xx, yy;
    int i, j, e;

    std::vector<Node> openSet;

    nodes = 0;

    // [clear previous paths]
    for (y = 0; y < ys; y++)
        for (x = 0; x < xs; x++)
            if (map[y][x] != A_star_wall)
                map[y][x] = A_star_space;
    // [A* changed points list]
    // init space for 2 points list
    _freepnt();
    int i0 = 0, i1 = xs * ys, n0 = 0, n1 = 0, ii;
    px = new int[i1 * 2];
    py = new int[i1 * 2];
    // if start is not on space then stop
    if (map[y0][x0] == A_star_space)
    {
        // init start position to first point list
        px[i0 + n0] = x0;
        py[i0 + n0] = y0;
        n0++;
        map[y0][x0] = 0;
        // search until hit the destination (swap point lists after each iteration and clear the second one)
        for (j = 1, e = 1; (e) && (map[y1][x1] == A_star_space); j++, ii = i0, i0 = i1, i1 = ii, n0 = n1, n1 = 0)
            // test neibours of all points in first list and add valid new points to second one
            for (e = 0, ii = i0; ii < i0 + n0; ii++)
            {
                nodes++;

                x = px[ii];
                y = py[ii];
                yy = y - 1;
                xx = x;
                if ((yy >= 0) && (map[yy][xx] == A_star_space))
                {
                    Node temp;
                    e = 1;
                    px[i1 + n1] = xx;
                    py[i1 + n1] = yy;
                    n1++;
                    map[yy][xx] = j;

                    //pg[i1 + n1] = j;
                    //ph[i1 + n1] = distance(xx, yy, x1, y1);

                    //map[yy][xx] = pg[i1 + n1] + ph[i1 + n1];
                }
                yy = y + 1;
                xx = x;
                if ((yy < ys) && (map[yy][xx] == A_star_space)) {
                    e = 1;
                    px[i1 + n1] = xx;
                    py[i1 + n1] = yy;
                    n1++;
                    map[yy][xx] = j;
                    //pg[i1 + n1] = j;
                    //ph[i1 + n1] = distance(xx, yy, x1, y1);

                    //map[yy][xx] = pg[i1 + n1] + ph[i1 + n1];
                }
                yy = y;
                xx = x - 1;
                if ((xx >= 0) && (map[yy][xx] == A_star_space)) {
                    e = 1;
                    px[i1 + n1] = xx;
                    py[i1 + n1] = yy;
                    n1++;
                    map[yy][xx] = j;
                    //pg[i1 + n1] = j;
                    //ph[i1 + n1] = distance(xx, yy, x1, y1);

                    //map[yy][xx] = pg[i1 + n1] + ph[i1 + n1];
                }
                yy = y;
                xx = x + 1;
                if ((xx < xs) && (map[yy][xx] == A_star_space)) {
                    e = 1;
                    px[i1 + n1] = xx;
                    py[i1 + n1] = yy;
                    n1++;
                    map[yy][xx] = j;
                    //pg[i1 + n1] = j;
                    //ph[i1 + n1] = distance(xx, yy, x1, y1);

                    //map[yy][xx] = pg[i1 + n1] + ph[i1 + n1];
                }
                //------------------------------------------------------------------------------------------------------
                //------------------------------------------------------------------------------------------------------
                
                yy = y - 1;
                xx = x - 1;
                if ((yy >= 0) && (xx >= 0) && (map[yy][xx] == A_star_space))
                {
                    e = 1;
                    px[i1 + n1] = xx;
                    py[i1 + n1] = yy;
                    n1++;
                    map[yy][xx] = j;
                    //pg[i1 + n1] = j;
                    //ph[i1 + n1] = distance(xx, yy, x1, y1);

                    //map[yy][xx] = pg[i1 + n1] + ph[i1 + n1];
                }
                yy = y - 1;
                xx = x + 1;
                if ((yy >= 0) && (xx < xs) && (map[yy][xx] == A_star_space)) {
                    e = 1;
                    px[i1 + n1] = xx;
                    py[i1 + n1] = yy;
                    n1++;
                    map[yy][xx] = j;
                    //pg[i1 + n1] = j;
                    //ph[i1 + n1] = distance(xx, yy, x1, y1);

                    //map[yy][xx] = pg[i1 + n1] + ph[i1 + n1];
                }
                yy = y + 1;
                xx = x - 1;
                if ((yy < ys) && (xx >= 0) && (map[yy][xx] == A_star_space)) {
                    e = 1;
                    px[i1 + n1] = xx;
                    py[i1 + n1] = yy;
                    n1++;
                    map[yy][xx] = j;
                    //pg[i1 + n1] = j;
                    //ph[i1 + n1] = distance(xx, yy, x1, y1);

                    //map[yy][xx] = pg[i1 + n1] + ph[i1 + n1];
                }
                yy = y + 1;
                xx = x + 1;
                if ((yy < ys) && (xx < xs) && (map[yy][xx] == A_star_space)) {
                    e = 1;
                    px[i1 + n1] = xx;
                    py[i1 + n1] = yy;
                    n1++;
                    map[yy][xx] = j;
                    //pg[i1 + n1] = j;
                    //ph[i1 + n1] = distance(xx, yy, x1, y1);

                    //map[yy][xx] = pg[i1 + n1] + ph[i1 + n1];
                }
                
            }
    }
    // [reconstruct path]
    _freepnt();
    if (map[y1][x1] == A_star_space) return std::vector<point>(0);
    if (map[y1][x1] == A_star_wall) return std::vector<point>(0);
    ps = map[y1][x1] + 1;
    px = new int[ps];
    py = new int[ps];

    std::vector<point> result;

    result.resize(ps);

    for (x = x1, y = y1, i = ps - 1, j = i - 1; i >= 0; i--, j--)
    {
        px[i] = x;
        py[i] = y;

        result[i] = point(px[i], py[i]);

        //std::cout << px[i] << ", " << py[i] << ", " << std::endl;

        if ((y > 0) && (map[y - 1][x] == j)) {
            y--;
            continue;
        }
        if ((y < ys - 1) && (map[y + 1][x] == j)) {
            y++;
            continue;
        }
        if ((x > 1) && (map[y][x - 1] == j)) {
            x--;
            continue;
        }
        if ((x < xs - 0) && (map[y][x + 1] == j)) {
            x++;
            continue;
        }
        break;
    }
    return result;
}
std::vector<point> A_star_fast::compute(int x0, int y0, int x1, int y1, float a)
{
    int x, y, xmin, xmax, ymin, ymax, xx, yy;
    int i, j, e;
    // [clear previous paths]
    for (y = 0; y < ys; y++)
        for (x = 0; x < xs; x++)
            if (map[y][x] != A_star_wall)
                map[y][x] = A_star_space;
    // [A* changed points list]
    // init space for 2 points list
    _freepnt();
    int i0 = 0, i1 = xs * ys, n0 = 0, n1 = 0, ii;
    px = new int[i1 * 2];
    py = new int[i1 * 2];
    // if start is not on space then stop
    if (map[y0][x0] == A_star_space)
    {
        // init start position to first point list
        px[i0 + n0] = x0;
        py[i0 + n0] = y0;
        n0++;
        map[y0][x0] = 0;
        // search until hit the destination (swap point lists after each iteration and clear the second one)
        for (j = 1, e = 1; (e) && (map[y1][x1] == A_star_space); j++, ii = i0, i0 = i1, i1 = ii, n0 = n1, n1 = 0)
            // test neibours of all points in first list and add valid new points to second one
            for (e = 0, ii = i0; ii < i0 + n0; ii++)
            {

                x = px[ii];
                y = py[ii];
                yy = y - 1;
                xx = x;
                if ((yy >= 0) && (map[yy][xx] == A_star_space))
                {
                    Node temp;
                    e = 1;
                    px[i1 + n1] = xx;
                    py[i1 + n1] = yy;
                    n1++;
                    map[yy][xx] = j;

                    //pg[i1 + n1] = j;
                    //ph[i1 + n1] = distance(xx, yy, x1, y1);

                    //map[yy][xx] = pg[i1 + n1] + ph[i1 + n1];
                }
                yy = y + 1;
                xx = x;
                if ((yy < ys) && (map[yy][xx] == A_star_space)) {
                    e = 1;
                    px[i1 + n1] = xx;
                    py[i1 + n1] = yy;
                    n1++;
                    map[yy][xx] = j;
                    //pg[i1 + n1] = j;
                    //ph[i1 + n1] = distance(xx, yy, x1, y1);

                    //map[yy][xx] = pg[i1 + n1] + ph[i1 + n1];
                }
                yy = y;
                xx = x - 1;
                if ((xx >= 0) && (map[yy][xx] == A_star_space)) {
                    e = 1;
                    px[i1 + n1] = xx;
                    py[i1 + n1] = yy;
                    n1++;
                    map[yy][xx] = j;
                    //pg[i1 + n1] = j;
                    //ph[i1 + n1] = distance(xx, yy, x1, y1);

                    //map[yy][xx] = pg[i1 + n1] + ph[i1 + n1];
                }
                yy = y;
                xx = x + 1;
                if ((xx < xs) && (map[yy][xx] == A_star_space)) {
                    e = 1;
                    px[i1 + n1] = xx;
                    py[i1 + n1] = yy;
                    n1++;
                    map[yy][xx] = j;
                    //pg[i1 + n1] = j;
                    //ph[i1 + n1] = distance(xx, yy, x1, y1);

                    //map[yy][xx] = pg[i1 + n1] + ph[i1 + n1];
                }
                //------------------------------------------------------------------------------------------------------
                //------------------------------------------------------------------------------------------------------

                yy = y - 1;
                xx = x - 1;
                if ((yy >= 0) && (xx >= 0) && (map[yy][xx] == A_star_space))
                {
                    e = 1;
                    px[i1 + n1] = xx;
                    py[i1 + n1] = yy;
                    n1++;
                    map[yy][xx] = j;
                    //pg[i1 + n1] = j;
                    //ph[i1 + n1] = distance(xx, yy, x1, y1);

                    //map[yy][xx] = pg[i1 + n1] + ph[i1 + n1];
                }
                yy = y - 1;
                xx = x + 1;
                if ((yy >= 0) && (xx < xs) && (map[yy][xx] == A_star_space)) {
                    e = 1;
                    px[i1 + n1] = xx;
                    py[i1 + n1] = yy;
                    n1++;
                    map[yy][xx] = j;
                    //pg[i1 + n1] = j;
                    //ph[i1 + n1] = distance(xx, yy, x1, y1);

                    //map[yy][xx] = pg[i1 + n1] + ph[i1 + n1];
                }
                yy = y + 1;
                xx = x - 1;
                if ((yy < ys) && (xx >= 0) && (map[yy][xx] == A_star_space)) {
                    e = 1;
                    px[i1 + n1] = xx;
                    py[i1 + n1] = yy;
                    n1++;
                    map[yy][xx] = j;
                    //pg[i1 + n1] = j;
                    //ph[i1 + n1] = distance(xx, yy, x1, y1);

                    //map[yy][xx] = pg[i1 + n1] + ph[i1 + n1];
                }
                yy = y + 1;
                xx = x + 1;
                if ((yy < ys) && (xx < xs) && (map[yy][xx] == A_star_space)) {
                    e = 1;
                    px[i1 + n1] = xx;
                    py[i1 + n1] = yy;
                    n1++;
                    map[yy][xx] = j;
                    //pg[i1 + n1] = j;
                    //ph[i1 + n1] = distance(xx, yy, x1, y1);

                    //map[yy][xx] = pg[i1 + n1] + ph[i1 + n1];
                }
            }
    }
    // [reconstruct path]
    _freepnt();
    if (map[y1][x1] == A_star_space) return std::vector<point>(0);
    if (map[y1][x1] == A_star_wall) return std::vector<point>(0);
    ps = map[y1][x1] + 1;
    px = new int[ps];
    py = new int[ps];
    for (i = 0; i < ps; i++) {
        px[i] = x0;
        py[i] = y0;
    }

    std::vector<point> result;

    result.resize(ps);

    for (x = x1, y = y1, i = ps - 1, j = i - 1; i >= 0; i--, j--)
    {
        px[i] = x;
        py[i] = y;

        result[i] = point(px[i], py[i]);

        //std::cout << px[i] << ", " << py[i] << ", " << std::endl;

        if ((y > 0) && (map[y - 1][x] == j)) {
            y--;
            continue;
        }
        if ((y < ys - 1) && (map[y + 1][x] == j)) {
            y++;
            continue;
        }
        if ((x > 1) && (map[y][x - 1] == j)) {
            x--;
            continue;
        }
        if ((x < xs - 0) && (map[y][x + 1] == j)) {
            x++;
            continue;
        }
        break;
    }
    return result;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
