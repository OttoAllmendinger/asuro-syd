
uint ldavg_on[2];
uint ldavg_off[2];

float kon=1.0;
float koff=1.0;

void calibrate_linedata(char led, uint* avg, float* k) {
    FrontLED(led);

    uint32_t ldsum[2] = {0,0};
    const int samples = 255;
    int i;

    for (i=0; i<samples; i++) {
        uint line_data[2];
        LineData(line_data);
        ldsum[0] += line_data[0];
        ldsum[1] += line_data[1];
    }

    avg[0] = (ldsum[0]/samples);
    avg[1] = (ldsum[1]/samples);
    if (avg[0]>0) {
        *k = ((float)avg[1])/((float)avg[0]);
    }
}

void ldfilter0(uint* ldin, int* ldout) {
    int i;
    for (i=0; i<2; i++) {
        ldout[i] = (int)ldin[i];
    }
}


void ldfilter1(uint* ldin, int* ldout, uint* ldavg) {
    int i;
    for (i=0; i<2; i++) {
        ldout[i] = (int)(ldin[i]-ldavg[i]);
    }
}

void ldfilter2(uint* ldin, int* ldout, float k) {
    ldout[0] = (int)(((float)ldout[0])*kon);
    ldout[1] = ldin[1];
}

void ldfilter3(uint* ldin, int* ldout, uint ldavg k) {
    ldout[0] = (int)(((float)ldout[0])*kon);
    ldout[1] = ldin[1];
}


void read_linedata() {
    uint ld_on[2];
    uint ld_off[2];
    int ld_diff[2];
    int ld_rl_diff;

    int ld_on_f[2];
    int ld_off_f[2];
    int ld_diff_f[2];
    int ld_rl_diff_f;

    FrontLED(OFF);
    LineData(ld_off);

    FrontLED(ON);
    LineData(ld_on);

    char filter = PollSwitch();

    if (filter==0) {
        ldfilter0(ld_on, ld_on_f);
        ldfilter0(ld_off, ld_off_f);
    } else if (filter==1) {
        ldfilter1(ld_on, ld_on_f, ldavg_on);
        ldfilter1(ld_off, ld_off_f, ldavg_off);
    } else if (filter==3) {
        ldfilter2(ld_on, ld_on_f, kon);
        ldfilter2(ld_off, ld_off_f, koff);
    }

    ld_diff[0] = ld_on[0] - ld_off[0];
    ld_diff[1] = ld_on[1] - ld_off[1];
    ld_rl_diff = ld_diff[0] - ld_diff[1];

    ld_diff_f[0] = ld_on_f[0] - ld_off_f[0];
    ld_diff_f[1] = ld_on_f[1] - ld_off_f[1];
    ld_rl_diff_f = ld_diff_f[0] - ld_diff_f[1];

    send(0, ld_off,   U2, 2);
    send(1, ld_on,    U2, 2);
    send(2, ld_diff,  S2, 2);
    send(3, &ld_rl_diff, S2, 1);

    send(4, ld_off_f,       S2, 2);
    send(5, ld_on_f,        S2, 2);
    send(6, ld_diff_f,      S2, 2);
    send(7, &ld_rl_diff_f,  S2, 1);
}


