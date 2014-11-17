#include "kohonen.h"

Kohonen::Kohonen()
{
}
Mat  Kohonen::getFrame(Mat parFrame)
{
    int map_dim=2;//размерность карты
    unsigned N[map_dim];
    unsigned u, v, q, index_winner;
    N[0] = 7;//количество нейронов в каждом слое
    N[1] = 7;

    //определение константы для вывода каждого класса определенным цветом
    uint32_t A=1;
    for (int i=0;i<map_dim; i++)
    {
        A*=N[i];//подсчет общего числа нейрнов
    }
    A=0xFFFFFF/A;

    fmll_som * som;
    Mat resultFrame;
    resultFrame.create(parFrame.rows, parFrame.cols,CV_8UC3);
    Mat frameFromUnsigned;
    frameFromUnsigned.create(parFrame.rows, parFrame.cols,CV_32S);

    Vec3b pixel;
    double ** vec;
    //инициализация векторного пространства значениями яркости изображения
    vec = (double **) fmll_alloc(sizeof(double), 2, parFrame.rows * parFrame.cols, 3);
    som = fmll_som_load(somFilename.c_str(),& fmll_distance_euclid, & fmll_distance_euclid);
    for(v = 0, q = 0; v < parFrame.rows; v++)
    for(u = 0; u < parFrame.cols; u++, q++)
    {
        pixel=parFrame.at<Vec3b>(v,u);

        vec[q][0] = pixel[0] / 255.;
        vec[q][1] = pixel[1] / 255.;
        vec[q][2] = pixel[2] / 255.;
    }
    //загрузка карты из xml
    if(som == NULL)
    {
        fmll_random * rnd;
        double param[2];
        param[0] = 0;
        param[1] = 0.01;
        //инициализация датчика случайных чисел
        rnd = fmll_random_init(FMLL_RANDOM_ALGORITHM_LCG, FMLL_RANDOM_DISTRIBUTION_UNIFORM, param, time(NULL));
        //иницилизация нейронной карты
        som = fmll_som_init(N, map_dim, 3, & fmll_distance_euclid, & fmll_distance_euclid);//(1)расстояние между векторами, 2)расстояние между нейронами.(2)не исп.,т.к. алгоритм WTA)
        //иницилизация весов синапсов нейронов карты случайными числами
        fmll_som_weight_init_random(som, rnd);

        cout<<"don't load"<<endl;

            //обучение нейронной карты
            fmll_som_so_kohonen(som, (const double **) vec, parFrame.rows * parFrame.cols,
                                        0.3, & fmll_timing_next_beta_step_plus_0_1, 0.8, 0.002, & fmll_som_neighbor_wta);
            if (fmll_som_save(som,somFilename.c_str())!=0)
            {
                cout<<"Ошибка сохранения Som в Xml"<<endl;
            }
            fmll_random_destroy(rnd);
   }
            for(v = 0, q = 0; v < parFrame.rows; v++)
            {
                for(u = 0; u < parFrame.cols; u++, q++)
                {
                   index_winner = fmll_som_run(som, vec[q]);
                   uint32_t color=A*index_winner;
                   pixel[0]=0x0000FF&color;
                   pixel[1]=(0x00FF00&color)>>8;
                   pixel[2]=(0xFF0000&color)>>16;
                   //pixel[0] = som->w[index_winner][0];
                   //pixel[1] = som->w[index_winner][1];
                   //pixel[2] = som->w[index_winner][2];
                   resultFrame.at<Vec3b>(v,u)=pixel;
                   frameFromUnsigned.at<uint32_t>(v,u)=color;
                   //cout<<color<<" ";
                }
                 //cout<<endl;
             }
            fmll_free(vec);
    imshow( "kohonen", resultFrame );
    fmll_som_destroy(som);
    return frameFromUnsigned;
}
