    #include <stdio.h>
    #include <math.h>

    #define TEST 0   // 1 - dla testowania, 0 - dla sprawdzarki

    #define RECURS_LEVEL_MAX  10
    #define N_MAX             10

    /////////////////////////////////////////////////////////////////////
    // 7.1 Calki jednokrotne

    // Przyklady podcalkowych funkcji jednej zmiennej

    typedef double (*Func1vFp)(double);  // Definiowanie nazwy dla typu wskaznika do funkcji jedej zmiennej 

    double f_poly(double x)
    {  //    polynomial  a[0]+a[1]x+ ... + a[n]x^n
        return 2 * pow(x, 5)- 4 * pow(x, 4) + 3.5 * pow(x, 2) + 1.35 * x - 6.25;
    }


    double f_rat(double x) 
    {  // Przyklad funkcji podcalkowej Proba z funkcja f(x) = 1/((x-0.5)*(x-0.5)+0.01) w przedziale od 0 do 3.
        return 1/(pow((x-0.5),2)+0.01);
    }

    double f_exp(double x) 
    {  // Przyklad funkcji podcalkowej
        return 2*x*exp((-1.5 * x)) - 1;
    }

    double f_trig(double x) 
    {  // Przyklad funkcji podcalkowej
        return x * tan(x);
    }

    // Obliczanie kwadratur złożonych dla funkcji jednej zmiennej

    double quad_rect_left(Func1vFp f1, double a, double b, int n) 
    {  // Prostokatow leftpoint 
        double sum = 0,interval = (b-a)/(double)n;
        for(int i = 0; i < n;i++)
        {
            sum += f1(a);
            a += interval;
        }
        return interval * sum;
    }

    double quad_rect_right(Func1vFp f1, double a, double b, int n)
    {  // Prostokatow rightpoint 
        double sum = 0,interval = (b-a)/(double)n;
        for(int i = 0; i < n;i++)
        {
            a+=interval;
            sum += f1(a);
        }
        return interval * sum;
    }

    double quad_rect_mid(Func1vFp f1, double a, double b, int n) 
    {  // Prostokatow midpoint 
        double sum = 0,interval = (b-a)/n;
        for(int i = 0; i < n;i++)
        {
            sum += f1((2*a+interval)/2);
            a += interval;
        }
        return interval * sum;
    }

    double quad_trap(Func1vFp func, double a, double b, int n) 
    {  // Trapezow
        double sum = 0,interval = (b-a)/n;
        for(int i = 0; i < n;i++)
        {
            sum += (func(a) + func(a+interval))/2;
            a += interval;
        }
        return interval * sum;
    }

    double quad_simpson(Func1vFp f, double a, double b, int n) 
    {  // Simpsona
        double sum = 0,interval = (b-a)/n;
        for(int i = 0; i < n;i++)
        {
            sum += (f(a) + 4*f((2*a+interval)/2) + f(a+interval))/6;
            a+=interval;
        }
        return interval * sum;
    }

    // Definiowanie nazwy dla typu wskaznika do funkcji obliczającej kwadraturę funkcji jednej zmiennej  
    typedef double (*QuadratureFp)(Func1vFp,double,double,int);  

    // Definicja i inicjowanie tablicy wskaznikow do funkcji jednej zmiennej, w kolejności: f_poly, f_rat, f_trig, f_exp.
    Func1vFp func_tab[4]={f_poly, f_rat, f_trig, f_exp};
    // Definicja i inicjowanie tablicy wskaznikow do kwadratur dla funkcji jednej zmiennej, w kolejności:
    // quad_rect_left, quad_rect_right, quad_rect_mid, quad_trap, quad_simpson.
    QuadratureFp quad_tab[5]={quad_rect_left, quad_rect_right, quad_rect_mid, quad_trap, quad_simpson};

    // Oblicza i zwraca wartosc wskazanej indeksem quad_no kwadratury dla wskazanej indeksem fun_no funkcji
    // w przedziale [a,b] z podzialem na n podprzedzialow. 
    double quad_select(int fun_no, int quad_no, double a, double b, int n) {
        return quad_tab[quad_no](func_tab[fun_no],a,b,n);
    }

     // Algorytm adaptacyjny obliczania kwadratury,
    double recurs(Func1vFp f, double a, double b, double S, double delta, QuadratureFp quad, int level) {
    double c = (a+b)/2.0f;
        double S1 = quad(f,a,(a+b)/2.0f,1),S2 = quad(f,(a+b)/2.0f,b,1);
        if(fabs(S-fabs(S1+S2)) < delta)
        {
            return S1+S2;
        }
        else
        {
            if(level == RECURS_LEVEL_MAX)
            {
                return NAN;
            }
            return recurs(f, a, (a+b)/2.0f, S1, delta/2.0f, quad, level + 1) + recurs(f, (a+b)/2.0f, b, S2, delta/2.0f, quad, level + 1);     
        }
    }

    // Funkcja inicjująca rekurencję
    double init_recurs(Func1vFp f, double a, double b, double delta, QuadratureFp quad) {
        double S = quad(f,a,b,1);
        return recurs(f,a,b,S,delta,quad,0);
    }

    ///////////////////////////////////////////////////////////////
    // 7.2 Calki dwukrotne:

    typedef double (*Func2vFp)(double,double);  // Definiowanie nazwy dla typu wskaznika do funkcji dwoch zmiennych

    // Przykladowe funkcje dwoch zmiennych:
    double func2v_1(double x, double y) {  //fxy1
        return sin(x/y);
    }
    double func2v_2(double x, double y) {
        return 2-x*x-y*y*y;
    }

    // Przykladowe funkcje brzegu obszaru calkowania (do calki podwojnej nad obszarem normalnym) 
    double lower_bound1(double x) {
        return sin(x);
    }
    double upper_bound1(double x) {
        return x*x+1;
    }
    double lower_bound2(double x) {
        return 0.7*exp(-2*x*x);
    }
    double upper_bound2(double x) {
        return sin(10*x);
    }

    // Metoda prostokatow (leftpoint) oblicza przyblizenie calki podwojnej nad obszarem prostokatnym
    double dbl_integr(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny)  {
        double intervalx = (x2-x1)/(double)nx, intervaly = (y2-y1)/(double)ny,sum = 0,xi=x1,yi=y1;
        for(int i = 0;i < nx;i++)
        {
            yi = y1;
            for(int j = 0;j < ny;j++)
            {
                sum += f(xi,yi);
                yi += intervaly;
            }
            xi += intervalx;
        }
        return intervalx * intervaly * sum;
    }

    // Oblicza kwadrature prostokatow midpoint dla calki podwojnej nad obszarem normalnym wzgledem osi 0x
    double dbl_integr_normal_1(Func2vFp f, double x1, double x2, int nx, double hy, Func1vFp fg, Func1vFp fh)  {
    double intervaly,intervalx = (x2 - x1)/nx, sum = 0,xi;

    for(int i = 0; i < nx; i++){
        xi = x1 + (i+0.5)*intervalx;

        int ny = (int)ceil((fh(xi) - fg(xi))/hy);
        intervaly = (fh(xi) - fg(xi))/ny;

        for(int j = 0; j < ny; j++){
            sum += intervaly*intervalx*f(xi, fg(xi) + j*intervaly + 0.5*intervaly);
        }
    }

    return sum;
    }

    // Oblicza kwadrature prostokatow leftpoint dla calki podwojnej nad obszarami normalnymi wzgledem osi 0x 
    double dbl_integr_normal_n(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny, Func1vFp fg, Func1vFp fh)  
    {
    double intervalx = (x2 - x1)/nx, intervaly = (y2 - y1)/ny, sum = 0,xi,new_y1,new_y2,new_intervaly;
    for(int i = 0; i < nx; i++){
        xi = x1 + i*intervalx;

        new_y1 = fmax(y1, fg(xi));
        new_y2 = fmin(y2, fh(xi));

        int new_ny = (int)ceil((new_y2 - new_y1)/intervaly);
        new_intervaly = (new_y2 - new_y1)/new_ny;

        for (int j = 0; j < new_ny; j++) {
            double yi = new_y1 + j * new_intervaly;

            if (fg(xi) <= yi && yi <= fh(xi))
                sum += intervalx * new_intervaly * f(xi, yi);
        }
    }

    return sum;
    }

    ///////////////////////////////////////////////////////////////
    // 7.3 Calki wielokrotne:

    typedef double (*FuncNvFp)(const double*, int);
    typedef int (*BoundNvFp)(const double*, int);

    // Przykladowa funkcja trzech zmiennych:
    double func3v(const double v[], int n) {
        return v[0]-v[1]+2*v[2];
    }

    // Przykładowy predykat w przestrzeni 3-wymiarowej
    int bound3v(const double v[], int n) {
        if(v[0]>0 && v[0]<0.5 && v[1]*v[1]+(v[2]-1)*(v[2]-1)<1) return 1;   // walec
        return 0;
    }

    // Przykladowa funkcja n zmiennych:
    double funcNv(const double v[], int n) {
        double fv=1.;
        for(int i=1; i<n; ++i) fv += sin(i*v[i]);   
        return fv;
    }
    // Przykładowy predykat w przestrzeni n-wymiarowej
    int boundNv(const double v[], int n) {
        double r=0.0;
        for(int i=0; i<n; ++i) r += (v[i]-1)*(v[i]-1);  // hiperkula n-wymiarowa
        if(r > 1.) return 0;
        return 1;
    }

    // Obliczanie calek wielokrotnych

double quad_rect_right3(FuncNvFp f, double a, double b, double *vars, int len, int n, BoundNvFp boundary) {
    double h=(b-a)/n,suma=0.0;
    vars[len-1]=a;
    for (int i=0; i<n; i++){
        vars[len-1]+=h;
        if (boundary==NULL || boundary(vars,len))
            suma+=h*f(vars,len);
    }
    return suma;
}

double trpl_quad_rect(FuncNvFp f, const double variable_lim[][2], const int tn[], BoundNvFp boundary) 
{     
    double x, y, z, hx, hy, hz, suma1 = 0.0, suma2 = 0.0;
    x = variable_lim[0][0];
    y = variable_lim[1][0];
    hx = (variable_lim[0][1] - x) / tn[0];
    hy = (variable_lim[1][1] - y) / tn[1];
    double vars[3] = {x, y};
    for (int i = 0; i < tn[0]; i++) {
        vars[0] = x + (i + 1) * hx;
        suma2 = 0.0;
        for (int j = 0; j < tn[1]; j++) {
            vars[1] = y + (j + 1) * hy; // Use right endpoint for y
            suma2 += hy * quad_rect_right3(f, variable_lim[2][0], variable_lim[2][1], vars, 3, tn[2], boundary);
        }
        suma1 += hx * suma2;
    }
    return suma1;
}


    // Oblicza calke wielokrotna (funkcji n zmiennych) "nad" n wymiarowym hiperprostopadloscianem z predykatem wykluczajacym jego czesci (jezeli boundary != NULL).
    // Metoda prostokatow midpoint wzdluz kazdej zmiennej.
    void recur_quad_rect_mid(double *psum, FuncNvFp f, int variable_no, double tvariable[], const double variable_lim[][2], const int tn[], int level, BoundNvFp boundary) 
    {
        #define VARIABLE_NO variable_no
        if(variable_no == level){
        double v[VARIABLE_NO];
        for(int i = 0; i < variable_no; i++){
            v[i] = tvariable[i];
        }
        double answer = 0;
        if((boundary != NULL && boundary(v, variable_no) != 0 ) || boundary == NULL)
            answer = f(v, variable_no);

        for(int i = 0; i < variable_no; i++){
            answer *= (variable_lim[i][1] - variable_lim[i][0])/tn[i];
        }

        *psum += answer;
    }
    else{
        double dx = (variable_lim[level][1] - variable_lim[level][0])/tn[level];
        for(int i = 0; i < tn[level]; i++){
            tvariable[level] = variable_lim[level][0] + (i+0.5)*dx;
            recur_quad_rect_mid(psum, f, variable_no, tvariable, variable_lim, tn, level + 1, boundary);
        }
    }
    }
    int main(void)
    {
        int to_do, n, nx, ny, integrand_fun_no, method_no, flag;
        double a,b,x1,x2,y1,y2,hy,sum,delta;
        double tvariable[N_MAX], variable_lim[N_MAX][2];
        int tn[N_MAX];

        if (TEST) printf("Wpisz numer testu [1, 7]: ");
        scanf("%d", &to_do);
        switch (to_do) {
        case 1: // 7.1.1 wybor funkcji i metody
            if (TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow: ");
            scanf("%lf %lf %d", &a, &b, &n);
            for(int q=0; q<5; ++q) {
                for(int f=0; f<4; ++f) {
                    printf("%.5f ",quad_select(f, q, a, b, n));
                }
                printf("\n");
            }
            break;
        case 2: // 7.1.2 rekurencyjny algorytm adaptacyjny
            if(TEST) printf("Nr funkcji (0-3) i metody (0-4): ");
            scanf("%d %d",&integrand_fun_no,&method_no);
            if (TEST) printf("Wpisz przedzial calkowania i tolerancje bledu: ");
            scanf("%lf %lf %lf", &a, &b, &delta);
            printf("%.5f\n",init_recurs(func_tab[integrand_fun_no],a,b,delta,quad_tab[method_no]));
            break;
        case 3: // 7.2.1 calka podwojna nad prostokatem
            if(TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow wzdluz x: ");
            scanf("%lf %lf %d",&x1,&x2,&nx);
            if(TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow wzdluz y: ");
            scanf("%lf %lf %d",&y1,&y2,&ny);
            printf("%.5f\n",dbl_integr(func2v_2, x1, x2, nx, y1, y2, ny));
            break;
        case 4: // 7.2.2 calka podwojna nad obszarem normalnym
            if(TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow zmiennej x: ");
            scanf("%lf %lf %d",&x1,&x2,&nx);
            if(TEST) printf("Wpisz dlugosc podprzedzialu wzdluz y: ");
            scanf("%lf",&hy);
            printf("%.5f\n",dbl_integr_normal_1(func2v_2, x1, x2, nx, hy, lower_bound2, upper_bound2));
            break;
        case 5: // 7.2.3 calka podwojna nad wieloma obszarami normalnymi
            if(TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow wzdluz x: ");
            scanf("%lf %lf %d",&x1,&x2,&nx);
            if(TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow wzdluz y: ");
            scanf("%lf %lf %d",&y1,&y2,&ny);
            printf("%.5f\n",dbl_integr_normal_n(func2v_2, x1, x2, nx, y1, y2, ny, lower_bound2, upper_bound2));
            break;
        case 6: // w7.3.1 calka potrojna po prostopadloscianie
            if(TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow 1. zmiennej: ");
            scanf("%lf %lf %d",&variable_lim[0][0],&variable_lim[0][1],tn);
            if(TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow 2. zmiennej: ");
            scanf("%lf %lf %d",&variable_lim[1][0],&variable_lim[1][1],tn+1);
            if(TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow 3. zmiennej: ");
            scanf("%lf %lf %d",&variable_lim[2][0],&variable_lim[2][1],tn+2);
            if(TEST) printf("Wpisz 1 gdy ograniczenie obszaru całkowania ma byc aktywne; 0 - w przeciwnym przypadku: ");
            scanf("%d",&flag);
            printf("%.5f\n",trpl_quad_rect(func3v, variable_lim, tn, flag?bound3v:NULL));
            break;
        case 7: // 7.3.3 calka n-wymiarowa na hiperprostopadloscianie
            if(TEST) printf("Wpisz liczbe zmiennych <= %d: ", N_MAX);
            scanf("%d",&n);
            if(n>N_MAX) break;
            for(int i=0; i<n; ++i) {
                if(TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow %d. zmiennej: ",i+1);
                scanf("%lf %lf %d",&variable_lim[i][0],&variable_lim[i][1],tn+i);
            }
            if(TEST) printf("Wpisz 1 gdy ograniczenie obszaru całkowania ma byc aktywne; 0 - w przeciwnym przypadku: ");
            scanf("%d",&flag);
            sum=0.;
            recur_quad_rect_mid(&sum, funcNv, n, tvariable, variable_lim, tn, 0, flag?boundNv:NULL);//        recur_quad_rect_mid(&sum, func3v_1, n, tvariable, variable_lim, tn, 0, flag?bound3v_2:NULL);
            printf("%.5f\n",sum);
            break;
        default:
            printf("Numer testu spoza zakresu [1, 7] %d", to_do);
        }
        return 0;
    }
