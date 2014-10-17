#include "kohonen.h"

Kohonen::Kohonen()
{
}
Mat  Kohonen::getFrame(Mat parFrame)
{
    int map_dim=2;
    unsigned N[map_dim];
    unsigned u, v, q, index_winner;
    N[0] = 7;
    N[1] = 7;

    int A=1;
    for (int i=0;i<map_dim; i++)
    {
        A*=N[i];
    }
    A=0xFFFFFF/A;
    fmll_som * som;
    fmll_random * rnd;
    double param[2];
    param[0] = 0;
    param[1] = 256;

    rnd = fmll_random_init(FMLL_RANDOM_ALGORITHM_LCG, FMLL_RANDOM_DISTRIBUTION_UNIFORM, param, time(NULL));
    som = fmll_som_init(N, map_dim, 3, & fmll_distance_euclid, & fmll_distance_euclid);
    fmll_som_weight_init_random(som, rnd);
    Mat resultFrame;
    resultFrame.create(parFrame.rows, parFrame.cols,CV_8UC3);

          Vec3b pixel;
          double ** vec;
          vec = (double **) fmll_alloc(sizeof(double), 2, parFrame.rows * parFrame.cols, 3);

            for(v = 0, q = 0; v < parFrame.rows; v++)
            for(u = 0; u < parFrame.cols; u++, q++)
            {
                pixel=parFrame.at<Vec3b>(v,u);

                vec[q][0] = pixel[0];
                vec[q][1] = pixel[1];
                vec[q][2] = pixel[2];
            }

            fmll_som_so_kohonen(som, (const double **) vec, parFrame.rows * parFrame.cols,
                                        0.3, & fmll_timing_next_beta_step_plus_0_1, 0.8, 0.002, & fmll_som_neighbor_wta);


            for(v = 0, q = 0; v < parFrame.rows; v++)
                for(u = 0; u < parFrame.cols; u++, q++)
                {
                   index_winner = fmll_som_run(som, vec[q]);
                   int color=A*index_winner;
                   pixel[0]=0x0000FF&color;
                   pixel[1]=(0x00FF00&color)>>8;
                   pixel[2]=(0xFF0000&color)>>16;
                   //pixel[0] = som->w[index_winner][0];
                   //pixel[1] = som->w[index_winner][1];
                   //pixel[2] = som->w[index_winner][2];

                   resultFrame.at<Vec3b>(v,u)=pixel;
                }
            fmll_free(vec);

    fmll_som_destroy(som);
    fmll_random_destroy(rnd);
    return resultFrame;
}
