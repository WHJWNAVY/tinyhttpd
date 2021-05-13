#!/usr/bin/awk

BEGIN {
    TBLH[1] = "PID";
    TBLH[2] = "USER";
    TBLH[3] = "PR";
    TBLH[4] = "NI";
    TBLH[5] = "VIRT";
    TBLH[6] = "RES";
    TBLH[7] = "SHR";
    TBLH[8] = "S";
    TBLH[9] = "%CPU";
    TBLH[10] = "%MEM";
    TBLH[11] = "TIME+";
    TBLH[12] = "COMMAND";
    printf("[\n");
    tbidx = 0;
}
{
    if (match($1, /[0 - 9]+/)) {
        if (tbidx > 0) {
            printf(",\n");
        }
        printf("{");
        for (i = 1; i <= NF; i++) {
            printf("\"%s\":\"%s\"", TBLH[i], $i);
            if (i < NF) {
                printf(",");
            }
        }
        printf("}");
        tbidx += 1;
    }
}
END {
    printf("\n]\n");
}