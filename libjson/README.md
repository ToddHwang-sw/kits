
## Print 

  ```sh

# ./json_main 
[JSON] 
[JSON] help
	LOAD   <json file name >
	PRINT  <INDEX>  [XML/FILE] [FILE NAME] 
	PATH   <INDEX>  
	FIND   <INDEX>  <PATH>  [PARTIAL] 
	DELETE <INDEX>  [PATH]  
	ADD    <INDEX>  <INDEX> 
	TREE   <INDEX>  [AMOUNT]
	RUN    <file> 
	DBG    
	MDBG   
	STATUS 
[JSON] 
[JSON] load test/1.json
Succeeded 0
[JSON] print 0

{
"firstName": "John",
"lastName": "doe",
"age": "26",
"address":  {
  "streetAddress": "naist street",
  "city": "Nara",
  "hello":  {
    "mygod":  {
      "vaccine":  {
        "job":  {
          "EEEE": "VAC"
          }
        }
      }
    },
  "XX":  {
    "PP":  ["HELLO", "MISTER", "KEY"],
    "ZZ":  ["11111", "22222", "33333", 
        {
        "BEEF":  {
          "STEAK": "WELLDONE"
          }
        }
      ],
    "YY":  [
        {
        "Cider":  {
          "Xyy": "Zyy",
          "Cyy": "Ayy"
          }
        },
        {
        "XCUB":  [
            {
            "Kara":  {
              "Eyy": "00000000",
              "Myy": "11111111"
              }
            },
            {
            "Memphis":  {
              "Jyy": "22222222222222",
              "Tyy": "33"
              }
            },
            {
            "Dorothy":  {
              "Vyy": "999999999",
              "Qyy": "7777777777"
              }
            }
          ]
        },
        {
        "GreenTea":  ["111111", "2222222", "333333", "444444"]
        },
        {
        "Beer":  {
          "EEE": "Zyy",
          "JJJ": "Ayy"
          }
        }
      ],
    "VV": "BBBB"
    },
  "postalCode": "630-0192"
  },
"aa":  {
  "bb": "bb11",
  "cX":  ["cc11", "cc22", "cc33", "cc44"],
  "dd": "dd11",
  "dx":  [
      {
      "d1":  {
        "d2":  {
          "d3":  {
            "d4": "fufufufuf"
            }
          }
        }
      },
      {
      "dn1":  {
        "dn2":  {
          "dn3":  {
            "dn4": "jujuju"
            }
          }
        }
      }
    ]
  },
"phoneNumbers":  [
    {
    "type": "iPhone",
    "number": "0123-4567-8888"
    },
    {
    "type": "home",
    "number": "0123-4567-8910"
    },
    {
    "type": "company",
    "teams":  [
        {
        "hr": "111-2222"
        },
        {
        "rnd": "2222-3333"
        },
        {
        "pm": "333-4444"
        }
      ]
    }
  ]
}

Succeeded 0
[JSON] 

  ```

## Browsing

  ```sh
[JSON] tree 0 
	P 0     0    <BLK  > BLK0                                                      0   <                    > <SMT0                > <                    >
	P 0     0    <STMT > SMT0                                                      0   <firstName           > <SMT1                > <BLK0                >
	P 1     0    <TOKN > firstName                                                 0   <John                > <                    > <SMT0                >
	P 2     0    <TOKN > John                                                      1   <                    > <                    > <SMT0                >
	P 0     0    <STMT > SMT1                                                      0   <lastName            > <SMT2                > <SMT0                >
	P 1     0    <TOKN > lastName                                                  0   <doe                 > <                    > <SMT1                >
	P 2     0    <TOKN > doe                                                       1   <                    > <                    > <SMT1                >
	P 0     0    <STMT > SMT2                                                      0   <age                 > <SMT53               > <SMT1                >
	P 1     0    <TOKN > age                                                       0   <26                  > <                    > <SMT2                >
	P 2     0    <TOKN > 26                                                        1   <                    > <                    > <SMT2                >
	P 0     0    <STMT > SMT53                                                     0   <address             > <SMT72               > <SMT2                >
	P 1     0    <TOKN > address                                                   0   <BLK40               > <                    > <SMT53               >
	P 2     0    <BLK  > BLK40                                                     0   <BLK1                > <                    > <SMT53               >
	P 3     1      <BLK  > BLK1                                                    1   <                    > <SMT3                > <BLK40               >
	P 3     1      <STMT > SMT3                                                    0   <streetAddress       > <SMT4                > <BLK1                >
	P 4     1      <TOKN > streetAddress                                           0   <naist street        > <                    > <SMT3                >
	P 5     1      <TOKN > naist street                                            1   <                    > <                    > <SMT3                >
	P 3     1      <STMT > SMT4                                                    0   <city                > <SMT9                > <SMT3                >
	P 4     1      <TOKN > city                                                    0   <Nara                > <                    > <SMT4                >
	P 5     1      <TOKN > Nara                                                    1   <                    > <                    > <SMT4                >
	P 3     1      <STMT > SMT9                                                    0   <hello               > <SMT51               > <SMT4                >
	P 4     1      <TOKN > hello                                                   0   <BLK9                > <                    > <SMT9                >
	P 5     1      <BLK  > BLK9                                                    0   <BLK2                > <                    > <SMT9                >
	P 6     2        <BLK  > BLK2                                                  1   <                    > <SMT8                > <BLK9                >
	P 6     2        <STMT > SMT8                                                  0   <mygod               > <                    > <BLK2                >
	P 7     2        <TOKN > mygod                                                 0   <BLK8                > <                    > <SMT8                >
	P 8     2        <BLK  > BLK8                                                  0   <BLK3                > <                    > <SMT8                >
	P 9     3          <BLK  > BLK3                                                0   <                    > <SMT7                > <BLK8                >
	P 9     3          <STMT > SMT7                                                0   <vaccine             > <                    > <BLK3                >
	P 10    3          <TOKN > vaccine                                             0   <BLK7                > <                    > <SMT7                >
	P 11    3          <BLK  > BLK7                                                0   <BLK4                > <                    > <SMT7                >
	P 12    4            <BLK  > BLK4                                              0   <                    > <SMT6                > <BLK7                >
	P 12    4            <STMT > SMT6                                              0   <job                 > <                    > <BLK4                >
	P 13    4            <TOKN > job                                               0   <BLK6                > <                    > <SMT6                >
	P 14    4            <BLK  > BLK6                                              0   <BLK5                > <                    > <SMT6                >
	P 15    5              <BLK  > BLK5                                            0   <                    > <SMT5                > <BLK6                >
	P 15    5              <STMT > SMT5                                            0   <EEEE                > <                    > <BLK5                >
	P 16    5              <TOKN > EEEE                                            0   <VAC                 > <                    > <SMT5                >
	P 17    5              <TOKN > VAC                                             0   <                    > <                    > <SMT5                >
	P 3     1      <STMT > SMT51                                                   0   <XX                  > <SMT52               > <SMT9                >
	P 4     1      <TOKN > XX                                                      0   <BLK39               > <                    > <SMT51               >
	P 5     1      <BLK  > BLK39                                                   0   <BLK10               > <                    > <SMT51               >
	P 6     2        <BLK  > BLK10                                                 1   <                    > <SMT13               > <BLK39               >
	P 6     2        <STMT > SMT13                                                 0   <PP                  > <SMT20               > <BLK10               >
	P 7     2        <TOKN > PP                                                    0   <SET1                > <                    > <SMT13               >
	P 8     2        <SET  > SET1                                                  0   <SET0                > <                    > <SMT13               >
	P 9     3          <SET  > SET0                                                1   <                    > <HELLO               > <SET1                >
	P 9     3          <TOKN > HELLO                                               0   <                    > <MISTER              > <SET0                >
	P 9     3          <TOKN > MISTER                                              0   <                    > <KEY                 > <SET0                >
	P 9     3          <TOKN > KEY                                                 0   <                    > <                    > <SET0                >
	P 6     2        <STMT > SMT20                                                 0   <ZZ                  > <SMT49               > <SMT13               >
	P 7     2        <TOKN > ZZ                                                    0   <SET3                > <                    > <SMT20               >
	P 8     2        <SET  > SET3                                                  0   <SET2                > <                    > <SMT20               >
	P 9     3          <SET  > SET2                                                1   <                    > <11111               > <SET3                >
	P 9     3          <TOKN > 11111                                               0   <                    > <22222               > <SET2                >
	P 9     3          <TOKN > 22222                                               0   <                    > <33333               > <SET2                >
	P 9     3          <TOKN > 33333                                               0   <                    > <BLK14               > <SET2                >
	P 9     3          <BLK  > BLK14                                               0   <BLK11               > <                    > <SET2                >
	P 10    4            <BLK  > BLK11                                             0   <                    > <SMT18               > <BLK14               >
	P 10    4            <STMT > SMT18                                             0   <BEEF                > <                    > <BLK11               >
	P 11    4            <TOKN > BEEF                                              0   <BLK13               > <                    > <SMT18               >
	P 12    4            <BLK  > BLK13                                             0   <BLK12               > <                    > <SMT18               >
	P 13    5              <BLK  > BLK12                                           0   <                    > <SMT17               > <BLK13               >
	P 13    5              <STMT > SMT17                                           0   <STEAK               > <                    > <BLK12               >
	P 14    5              <TOKN > STEAK                                           0   <WELLDONE            > <                    > <SMT17               >
	P 15    5              <TOKN > WELLDONE                                        0   <                    > <                    > <SMT17               >
	P 6     2        <STMT > SMT49                                                 0   <YY                  > <SMT50               > <SMT20               >
	P 7     2        <TOKN > YY                                                    0   <SET9                > <                    > <SMT49               >
	P 8     2        <SET  > SET9                                                  0   <SET4                > <                    > <SMT49               >
	P 9     3          <SET  > SET4                                                1   <                    > <BLK18               > <SET9                >
	P 9     3          <BLK  > BLK18                                               0   <BLK15               > <BLK32               > <SET4                >
	P 10    4            <BLK  > BLK15                                             1   <                    > <SMT23               > <BLK18               >
	P 10    4            <STMT > SMT23                                             0   <Cider               > <                    > <BLK15               >
	P 11    4            <TOKN > Cider                                             0   <BLK17               > <                    > <SMT23               >
	P 12    4            <BLK  > BLK17                                             0   <BLK16               > <                    > <SMT23               >
	P 13    5              <BLK  > BLK16                                           0   <                    > <SMT21               > <BLK17               >
	P 13    5              <STMT > SMT21                                           0   <Xyy                 > <SMT22               > <BLK16               >
	P 14    5              <TOKN > Xyy                                             0   <Zyy                 > <                    > <SMT21               >
	P 15    5              <TOKN > Zyy                                             1   <                    > <                    > <SMT21               >
	P 13    5              <STMT > SMT22                                           0   <Cyy                 > <                    > <SMT21               >
	P 14    5              <TOKN > Cyy                                             0   <Ayy                 > <                    > <SMT22               >
	P 15    5              <TOKN > Ayy                                             0   <                    > <                    > <SMT22               >
	P 9     3          <BLK  > BLK32                                               0   <BLK19               > <BLK34               > <SET4                >
	P 10    4            <BLK  > BLK19                                             1   <                    > <SMT37               > <BLK32               >
	P 10    4            <STMT > SMT37                                             0   <XCUB                > <                    > <BLK19               >
	P 11    4            <TOKN > XCUB                                              0   <SET6                > <                    > <SMT37               >
	P 12    4            <SET  > SET6                                              0   <SET5                > <                    > <SMT37               >
	P 13    5              <SET  > SET5                                            0   <                    > <BLK23               > <SET6                >
	P 13    5              <BLK  > BLK23                                           0   <BLK20               > <BLK27               > <SET5                >
	P 14    6                <BLK  > BLK20                                         1   <                    > <SMT27               > <BLK23               >
	P 14    6                <STMT > SMT27                                         0   <Kara                > <                    > <BLK20               >
	P 15    6                <TOKN > Kara                                          0   <BLK22               > <                    > <SMT27               >
	P 16    6                <BLK  > BLK22                                         0   <BLK21               > <                    > <SMT27               >
	P 17    7                  <BLK  > BLK21                                       0   <                    > <SMT25               > <BLK22               >
	P 17    7                  <STMT > SMT25                                       0   <Eyy                 > <SMT26               > <BLK21               >
	P 18    7                  <TOKN > Eyy                                         0   <00000000            > <                    > <SMT25               >
	P 19    7                  <TOKN > 00000000                                    1   <                    > <                    > <SMT25               >
	P 17    7                  <STMT > SMT26                                       0   <Myy                 > <                    > <SMT25               >
	P 18    7                  <TOKN > Myy                                         0   <11111111            > <                    > <SMT26               >
	P 19    7                  <TOKN > 11111111                                    0   <                    > <                    > <SMT26               >
	P 13    5              <BLK  > BLK27                                           0   <BLK24               > <BLK31               > <SET5                >
	P 14    6                <BLK  > BLK24                                         1   <                    > <SMT31               > <BLK27               >
	P 14    6                <STMT > SMT31                                         0   <Memphis             > <                    > <BLK24               >
	P 15    6                <TOKN > Memphis                                       0   <BLK26               > <                    > <SMT31               >
	P 16    6                <BLK  > BLK26                                         0   <BLK25               > <                    > <SMT31               >
	P 17    7                  <BLK  > BLK25                                       0   <                    > <SMT29               > <BLK26               >
	P 17    7                  <STMT > SMT29                                       0   <Jyy                 > <SMT30               > <BLK25               >
	P 18    7                  <TOKN > Jyy                                         0   <22222222222222      > <                    > <SMT29               >
	P 19    7                  <TOKN > 22222222222222                              1   <                    > <                    > <SMT29               >
	P 17    7                  <STMT > SMT30                                       0   <Tyy                 > <                    > <SMT29               >
	P 18    7                  <TOKN > Tyy                                         0   <33                  > <                    > <SMT30               >
	P 19    7                  <TOKN > 33                                          0   <                    > <                    > <SMT30               >
	P 13    5              <BLK  > BLK31                                           0   <BLK28               > <                    > <SET5                >
	P 14    6                <BLK  > BLK28                                         0   <                    > <SMT35               > <BLK31               >
	P 14    6                <STMT > SMT35                                         0   <Dorothy             > <                    > <BLK28               >
	P 15    6                <TOKN > Dorothy                                       0   <BLK30               > <                    > <SMT35               >
	P 16    6                <BLK  > BLK30                                         0   <BLK29               > <                    > <SMT35               >
	P 17    7                  <BLK  > BLK29                                       0   <                    > <SMT33               > <BLK30               >
	P 17    7                  <STMT > SMT33                                       0   <Vyy                 > <SMT34               > <BLK29               >
	P 18    7                  <TOKN > Vyy                                         0   <999999999           > <                    > <SMT33               >
	P 19    7                  <TOKN > 999999999                                   1   <                    > <                    > <SMT33               >
	P 17    7                  <STMT > SMT34                                       0   <Qyy                 > <                    > <SMT33               >
	P 18    7                  <TOKN > Qyy                                         0   <7777777777          > <                    > <SMT34               >
	P 19    7                  <TOKN > 7777777777                                  0   <                    > <                    > <SMT34               >
	P 9     3          <BLK  > BLK34                                               0   <BLK33               > <BLK38               > <SET4                >
	P 10    4            <BLK  > BLK33                                             1   <                    > <SMT43               > <BLK34               >
	P 10    4            <STMT > SMT43                                             0   <GreenTea            > <                    > <BLK33               >
	P 11    4            <TOKN > GreenTea                                          0   <SET8                > <                    > <SMT43               >
	P 12    4            <SET  > SET8                                              0   <SET7                > <                    > <SMT43               >
	P 13    5              <SET  > SET7                                            0   <                    > <111111              > <SET8                >
	P 13    5              <TOKN > 111111                                          0   <                    > <2222222             > <SET7                >
	P 13    5              <TOKN > 2222222                                         0   <                    > <333333              > <SET7                >
	P 13    5              <TOKN > 333333                                          0   <                    > <444444              > <SET7                >
	P 13    5              <TOKN > 444444                                          0   <                    > <                    > <SET7                >
	P 9     3          <BLK  > BLK38                                               0   <BLK35               > <                    > <SET4                >
	P 10    4            <BLK  > BLK35                                             0   <                    > <SMT47               > <BLK38               >
	P 10    4            <STMT > SMT47                                             0   <Beer                > <                    > <BLK35               >
	P 11    4            <TOKN > Beer                                              0   <BLK37               > <                    > <SMT47               >
	P 12    4            <BLK  > BLK37                                             0   <BLK36               > <                    > <SMT47               >
	P 13    5              <BLK  > BLK36                                           0   <                    > <SMT45               > <BLK37               >
	P 13    5              <STMT > SMT45                                           0   <EEE                 > <SMT46               > <BLK36               >
	P 14    5              <TOKN > EEE                                             0   <Zyy                 > <                    > <SMT45               >
	P 15    5              <TOKN > Zyy                                             1   <                    > <                    > <SMT45               >
	P 13    5              <STMT > SMT46                                           0   <JJJ                 > <                    > <SMT45               >
	P 14    5              <TOKN > JJJ                                             0   <Ayy                 > <                    > <SMT46               >
	P 15    5              <TOKN > Ayy                                             0   <                    > <                    > <SMT46               >
	P 6     2        <STMT > SMT50                                                 0   <VV                  > <                    > <SMT49               >
	P 7     2        <TOKN > VV                                                    0   <BBBB                > <                    > <SMT50               >
	P 8     2        <TOKN > BBBB                                                  0   <                    > <                    > <SMT50               >
	P 3     1      <STMT > SMT52                                                   0   <postalCode          > <                    > <SMT51               >
	P 4     1      <TOKN > postalCode                                              0   <630-0192            > <                    > <SMT52               >
	P 5     1      <TOKN > 630-0192                                                0   <                    > <                    > <SMT52               >
	P 0     0    <STMT > SMT72                                                     0   <aa                  > <SMT88               > <SMT53               >
	P 1     0    <TOKN > aa                                                        0   <BLK58               > <                    > <SMT72               >
	P 2     0    <BLK  > BLK58                                                     0   <BLK41               > <                    > <SMT72               >
	P 3     1      <BLK  > BLK41                                                   1   <                    > <SMT54               > <BLK58               >
	P 3     1      <STMT > SMT54                                                   0   <bb                  > <SMT59               > <BLK41               >
	P 4     1      <TOKN > bb                                                      0   <bb11                > <                    > <SMT54               >
	P 5     1      <TOKN > bb11                                                    1   <                    > <                    > <SMT54               >
	P 3     1      <STMT > SMT59                                                   0   <cX                  > <SMT60               > <SMT54               >
	P 4     1      <TOKN > cX                                                      0   <SET11               > <                    > <SMT59               >
	P 5     1      <SET  > SET11                                                   0   <SET10               > <                    > <SMT59               >
	P 6     2        <SET  > SET10                                                 1   <                    > <cc11                > <SET11               >
	P 6     2        <TOKN > cc11                                                  0   <                    > <cc22                > <SET10               >
	P 6     2        <TOKN > cc22                                                  0   <                    > <cc33                > <SET10               >
	P 6     2        <TOKN > cc33                                                  0   <                    > <cc44                > <SET10               >
	P 6     2        <TOKN > cc44                                                  0   <                    > <                    > <SET10               >
	P 3     1      <STMT > SMT60                                                   0   <dd                  > <SMT71               > <SMT59               >
	P 4     1      <TOKN > dd                                                      0   <dd11                > <                    > <SMT60               >
	P 5     1      <TOKN > dd11                                                    1   <                    > <                    > <SMT60               >
	P 3     1      <STMT > SMT71                                                   0   <dx                  > <                    > <SMT60               >
	P 4     1      <TOKN > dx                                                      0   <SET13               > <                    > <SMT71               >
	P 5     1      <SET  > SET13                                                   0   <SET12               > <                    > <SMT71               >
	P 6     2        <SET  > SET12                                                 0   <                    > <BLK49               > <SET13               >
	P 6     2        <BLK  > BLK49                                                 0   <BLK42               > <BLK57               > <SET12               >
	P 7     3          <BLK  > BLK42                                               1   <                    > <SMT64               > <BLK49               >
	P 7     3          <STMT > SMT64                                               0   <d1                  > <                    > <BLK42               >
	P 8     3          <TOKN > d1                                                  0   <BLK48               > <                    > <SMT64               >
	P 9     3          <BLK  > BLK48                                               0   <BLK43               > <                    > <SMT64               >
	P 10    4            <BLK  > BLK43                                             0   <                    > <SMT63               > <BLK48               >
	P 10    4            <STMT > SMT63                                             0   <d2                  > <                    > <BLK43               >
	P 11    4            <TOKN > d2                                                0   <BLK47               > <                    > <SMT63               >
	P 12    4            <BLK  > BLK47                                             0   <BLK44               > <                    > <SMT63               >
	P 13    5              <BLK  > BLK44                                           0   <                    > <SMT62               > <BLK47               >
	P 13    5              <STMT > SMT62                                           0   <d3                  > <                    > <BLK44               >
	P 14    5              <TOKN > d3                                              0   <BLK46               > <                    > <SMT62               >
	P 15    5              <BLK  > BLK46                                           0   <BLK45               > <                    > <SMT62               >
	P 16    6                <BLK  > BLK45                                         0   <                    > <SMT61               > <BLK46               >
	P 16    6                <STMT > SMT61                                         0   <d4                  > <                    > <BLK45               >
	P 17    6                <TOKN > d4                                            0   <fufufufuf           > <                    > <SMT61               >
	P 18    6                <TOKN > fufufufuf                                     0   <                    > <                    > <SMT61               >
	P 6     2        <BLK  > BLK57                                                 0   <BLK50               > <                    > <SET12               >
	P 7     3          <BLK  > BLK50                                               0   <                    > <SMT69               > <BLK57               >
	P 7     3          <STMT > SMT69                                               0   <dn1                 > <                    > <BLK50               >
	P 8     3          <TOKN > dn1                                                 0   <BLK56               > <                    > <SMT69               >
	P 9     3          <BLK  > BLK56                                               0   <BLK51               > <                    > <SMT69               >
	P 10    4            <BLK  > BLK51                                             0   <                    > <SMT68               > <BLK56               >
	P 10    4            <STMT > SMT68                                             0   <dn2                 > <                    > <BLK51               >
	P 11    4            <TOKN > dn2                                               0   <BLK55               > <                    > <SMT68               >
	P 12    4            <BLK  > BLK55                                             0   <BLK52               > <                    > <SMT68               >
	P 13    5              <BLK  > BLK52                                           0   <                    > <SMT67               > <BLK55               >
	P 13    5              <STMT > SMT67                                           0   <dn3                 > <                    > <BLK52               >
	P 14    5              <TOKN > dn3                                             0   <BLK54               > <                    > <SMT67               >
	P 15    5              <BLK  > BLK54                                           0   <BLK53               > <                    > <SMT67               >
	P 16    6                <BLK  > BLK53                                         0   <                    > <SMT66               > <BLK54               >
	P 16    6                <STMT > SMT66                                         0   <dn4                 > <                    > <BLK53               >
	P 17    6                <TOKN > dn4                                           0   <jujuju              > <                    > <SMT66               >
	P 18    6                <TOKN > jujuju                                        0   <                    > <                    > <SMT66               >
	P 0     0    <STMT > SMT88                                                     0   <phoneNumbers        > <                    > <SMT72               >
	P 1     0    <TOKN > phoneNumbers                                              0   <SET17               > <                    > <SMT88               >
	P 2     0    <SET  > SET17                                                     0   <SET14               > <                    > <SMT88               >
	P 3     1      <SET  > SET14                                                   0   <                    > <BLK60               > <SET17               >
	P 3     1      <BLK  > BLK60                                                   0   <BLK59               > <BLK62               > <SET14               >
	P 4     2        <BLK  > BLK59                                                 1   <                    > <SMT73               > <BLK60               >
	P 4     2        <STMT > SMT73                                                 0   <type                > <SMT74               > <BLK59               >
	P 5     2        <TOKN > type                                                  0   <iPhone              > <                    > <SMT73               >
	P 6     2        <TOKN > iPhone                                                1   <                    > <                    > <SMT73               >
	P 4     2        <STMT > SMT74                                                 0   <number              > <                    > <SMT73               >
	P 5     2        <TOKN > number                                                0   <0123-4567-8888      > <                    > <SMT74               >
	P 6     2        <TOKN > 0123-4567-8888                                        0   <                    > <                    > <SMT74               >
	P 3     1      <BLK  > BLK62                                                   0   <BLK61               > <BLK70               > <SET14               >
	P 4     2        <BLK  > BLK61                                                 1   <                    > <SMT76               > <BLK62               >
	P 4     2        <STMT > SMT76                                                 0   <type                > <SMT77               > <BLK61               >
	P 5     2        <TOKN > type                                                  0   <home                > <                    > <SMT76               >
	P 6     2        <TOKN > home                                                  1   <                    > <                    > <SMT76               >
	P 4     2        <STMT > SMT77                                                 0   <number              > <                    > <SMT76               >
	P 5     2        <TOKN > number                                                0   <0123-4567-8910      > <                    > <SMT77               >
	P 6     2        <TOKN > 0123-4567-8910                                        0   <                    > <                    > <SMT77               >
	P 3     1      <BLK  > BLK70                                                   0   <BLK63               > <                    > <SET14               >
	P 4     2        <BLK  > BLK63                                                 0   <                    > <SMT79               > <BLK70               >
	P 4     2        <STMT > SMT79                                                 0   <type                > <SMT86               > <BLK63               >
	P 5     2        <TOKN > type                                                  0   <company             > <                    > <SMT79               >
	P 6     2        <TOKN > company                                               1   <                    > <                    > <SMT79               >
	P 4     2        <STMT > SMT86                                                 0   <teams               > <                    > <SMT79               >
	P 5     2        <TOKN > teams                                                 0   <SET16               > <                    > <SMT86               >
	P 6     2        <SET  > SET16                                                 0   <SET15               > <                    > <SMT86               >
	P 7     3          <SET  > SET15                                               0   <                    > <BLK65               > <SET16               >
	P 7     3          <BLK  > BLK65                                               0   <BLK64               > <BLK67               > <SET15               >
	P 8     4            <BLK  > BLK64                                             1   <                    > <SMT80               > <BLK65               >
	P 8     4            <STMT > SMT80                                             0   <hr                  > <                    > <BLK64               >
	P 9     4            <TOKN > hr                                                0   <111-2222            > <                    > <SMT80               >
	P 10    4            <TOKN > 111-2222                                          0   <                    > <                    > <SMT80               >
	P 7     3          <BLK  > BLK67                                               0   <BLK66               > <BLK69               > <SET15               >
	P 8     4            <BLK  > BLK66                                             1   <                    > <SMT82               > <BLK67               >
	P 8     4            <STMT > SMT82                                             0   <rnd                 > <                    > <BLK66               >
	P 9     4            <TOKN > rnd                                               0   <2222-3333           > <                    > <SMT82               >
	P 10    4            <TOKN > 2222-3333                                         0   <                    > <                    > <SMT82               >
	P 7     3          <BLK  > BLK69                                               0   <BLK68               > <                    > <SET15               >
	P 8     4            <BLK  > BLK68                                             0   <                    > <SMT84               > <BLK69               >
	P 8     4            <STMT > SMT84                                             0   <pm                  > <                    > <BLK68               >
	P 9     4            <TOKN > pm                                                0   <333-4444            > <                    > <SMT84               >
	P 10    4            <TOKN > 333-4444                                          0   <                    > <                    > <SMT84               >

[JSON] 

  ```

## Subtree Copy 

  ```sh
# ./json_main 
[JSON] load test/1.json
Succeeded 0
[JSON] print 0

{
"firstName": "John",
"lastName": "doe",
"age": "26",
"address":  {
  "streetAddress": "naist street",
  "city": "Nara",
  "hello":  {
    "mygod":  {
      "vaccine":  {
        "job":  {
          "EEEE": "VAC"
          }
        }
      }
    },
  "XX":  {
    "PP":  ["HELLO", "MISTER", "KEY"],
    "ZZ":  ["11111", "22222", "33333", 
        {
        "BEEF":  {
          "STEAK": "WELLDONE"
          }
        }
      ],
    "YY":  [
        {
        "Cider":  {
          "Xyy": "Zyy",
          "Cyy": "Ayy"
          }
        },
        {
        "XCUB":  [
            {
            "Kara":  {
              "Eyy": "00000000",
              "Myy": "11111111"
              }
            },
            {
            "Memphis":  {
              "Jyy": "22222222222222",
              "Tyy": "33"
              }
            },
            {
            "Dorothy":  {
              "Vyy": "999999999",
              "Qyy": "7777777777"
              }
            }
          ]
        },
        {
        "GreenTea":  ["111111", "2222222", "333333", "444444"]
        },
        {
        "Beer":  {
          "EEE": "Zyy",
          "JJJ": "Ayy"
          }
        }
      ],
    "VV": "BBBB"
    },
  "postalCode": "630-0192"
  },
"aa":  {
  "bb": "bb11",
  "cX":  ["cc11", "cc22", "cc33", "cc44"],
  "dd": "dd11",
  "dx":  [
      {
      "d1":  {
        "d2":  {
          "d3":  {
            "d4": "fufufufuf"
            }
          }
        }
      },
      {
      "dn1":  {
        "dn2":  {
          "dn3":  {
            "dn4": "jujuju"
            }
          }
        }
      }
    ]
  },
"phoneNumbers":  [
    {
    "type": "iPhone",
    "number": "0123-4567-8888"
    },
    {
    "type": "home",
    "number": "0123-4567-8910"
    },
    {
    "type": "company",
    "teams":  [
        {
        "hr": "111-2222"
        },
        {
        "rnd": "2222-3333"
        },
        {
        "pm": "333-4444"
        }
      ]
    }
  ]
}

Succeeded 0

[JSON] path 0
   T firstName                               
   T lastName                                
   T age                                     
     address                                 
   T address.streetAddress                   
   T address.city                            
     address.hello                           
     address.hello.mygod                     
     address.hello.mygod.vaccine             
     address.hello.mygod.vaccine.job         
   T address.hello.mygod.vaccine.job.EEEE    
     address.XX                              
     address.XX.PP                           
   T address.XX.PP[0]                        
   T address.XX.PP[1]                        
   T address.XX.PP[2]                        
     address.XX.ZZ                           
   T address.XX.ZZ[0]                        
   T address.XX.ZZ[1]                        
   T address.XX.ZZ[2]                        
     address.XX.ZZ[3].BEEF                   
   T address.XX.ZZ[3].BEEF.STEAK             
     address.XX.YY                           
     address.XX.YY[0].Cider                  
   T address.XX.YY[0].Cider.Xyy              
   T address.XX.YY[0].Cider.Cyy              
     address.XX.YY[1].XCUB                   
     address.XX.YY[1].XCUB[0].Kara           
   T address.XX.YY[1].XCUB[0].Kara.Eyy       
   T address.XX.YY[1].XCUB[0].Kara.Myy       
     address.XX.YY[1].XCUB[1].Memphis        
   T address.XX.YY[1].XCUB[1].Memphis.Jyy    
   T address.XX.YY[1].XCUB[1].Memphis.Tyy    
     address.XX.YY[1].XCUB[2].Dorothy        
   T address.XX.YY[1].XCUB[2].Dorothy.Vyy    
   T address.XX.YY[1].XCUB[2].Dorothy.Qyy    
     address.XX.YY[2].GreenTea               
   T address.XX.YY[2].GreenTea[0]            
   T address.XX.YY[2].GreenTea[1]            
   T address.XX.YY[2].GreenTea[2]            
   T address.XX.YY[2].GreenTea[3]            
     address.XX.YY[3].Beer                   
   T address.XX.YY[3].Beer.EEE               
   T address.XX.YY[3].Beer.JJJ               
   T address.XX.VV                           
   T address.postalCode                      
     aa                                      
   T aa.bb                                   
     aa.cX                                   
   T aa.cX[0]                                
   T aa.cX[1]                                
   T aa.cX[2]                                
   T aa.cX[3]                                
   T aa.dd                                   
     aa.dx                                   
     aa.dx[0].d1                             
     aa.dx[0].d1.d2                          
     aa.dx[0].d1.d2.d3                       
   T aa.dx[0].d1.d2.d3.d4                    
     aa.dx[1].dn1                            
     aa.dx[1].dn1.dn2                        
     aa.dx[1].dn1.dn2.dn3                    
   T aa.dx[1].dn1.dn2.dn3.dn4                
     phoneNumbers                            
   T phoneNumbers[0].type                    
   T phoneNumbers[0].number                  
   T phoneNumbers[1].type                    
   T phoneNumbers[1].number                  
   T phoneNumbers[2].type                    
     phoneNumbers[2].teams                   
   T phoneNumbers[2].teams[0].hr             
   T phoneNumbers[2].teams[1].rnd            
   T phoneNumbers[2].teams[2].pm             
Succeeded 0
[JSON] find 0 address.XX.YY
Succeeded 1
[JSON] print 1

{
"address":  {
  "XX":  {
    "YY":  [
        {
        "Cider":  {
          "Xyy": "Zyy",
          "Cyy": "Ayy"
          }
        },
        {
        "XCUB":  [
            {
            "Kara":  {
              "Eyy": "00000000",
              "Myy": "11111111"
              }
            },
            {
            "Memphis":  {
              "Jyy": "22222222222222",
              "Tyy": "33"
              }
            },
            {
            "Dorothy":  {
              "Vyy": "999999999",
              "Qyy": "7777777777"
              }
            }
          ]
        },
        {
        "GreenTea":  ["111111", "2222222", "333333", "444444"]
        },
        {
        "Beer":  {
          "EEE": "Zyy",
          "JJJ": "Ayy"
          }
        }
      ]
    }
  }
}

Succeeded 1
[JSON] path 1
     address                                 
     address.XX                              
     address.XX.YY                           
     address.XX.YY[0].Cider                  
   T address.XX.YY[0].Cider.Xyy              
   T address.XX.YY[0].Cider.Cyy              
     address.XX.YY[1].XCUB                   
     address.XX.YY[1].XCUB[0].Kara           
   T address.XX.YY[1].XCUB[0].Kara.Eyy       
   T address.XX.YY[1].XCUB[0].Kara.Myy       
     address.XX.YY[1].XCUB[1].Memphis        
   T address.XX.YY[1].XCUB[1].Memphis.Jyy    
   T address.XX.YY[1].XCUB[1].Memphis.Tyy    
     address.XX.YY[1].XCUB[2].Dorothy        
   T address.XX.YY[1].XCUB[2].Dorothy.Vyy    
   T address.XX.YY[1].XCUB[2].Dorothy.Qyy    
     address.XX.YY[2].GreenTea               
   T address.XX.YY[2].GreenTea[0]            
   T address.XX.YY[2].GreenTea[1]            
   T address.XX.YY[2].GreenTea[2]            
   T address.XX.YY[2].GreenTea[3]            
     address.XX.YY[3].Beer                   
   T address.XX.YY[3].Beer.EEE               
   T address.XX.YY[3].Beer.JJJ               
Succeeded 1
[JSON] 

  ```

## Subtree Deletion

  ```sh

# ./json_main 
[JSON] load test/1.json
Succeeded 0
[JSON] find 0 address.XX.YY
Succeeded 1
[JSON] print 1

{
"address":  {
  "XX":  {
    "YY":  [
        {
        "Cider":  {
          "Xyy": "Zyy",
          "Cyy": "Ayy"
          }
        },
        {
        "XCUB":  [
            {
            "Kara":  {
              "Eyy": "00000000",
              "Myy": "11111111"
              }
            },
            {
            "Memphis":  {
              "Jyy": "22222222222222",
              "Tyy": "33"
              }
            },
            {
            "Dorothy":  {
              "Vyy": "999999999",
              "Qyy": "7777777777"
              }
            }
          ]
        },
        {
        "GreenTea":  ["111111", "2222222", "333333", "444444"]
        },
        {
        "Beer":  {
          "EEE": "Zyy",
          "JJJ": "Ayy"
          }
        }
      ]
    }
  }
}

Succeeded 1
[JSON] print 0

{
"firstName": "John",
"lastName": "doe",
"age": "26",
"address":  {
  "streetAddress": "naist street",
  "city": "Nara",
  "hello":  {
    "mygod":  {
      "vaccine":  {
        "job":  {
          "EEEE": "VAC"
          }
        }
      }
    },
  "XX":  {
    "PP":  ["HELLO", "MISTER", "KEY"],
    "ZZ":  ["11111", "22222", "33333", 
        {
        "BEEF":  {
          "STEAK": "WELLDONE"
          }
        }
      ],
    "YY":  [
        {
        "Cider":  {
          "Xyy": "Zyy",
          "Cyy": "Ayy"
          }
        },
        {
        "XCUB":  [
            {
            "Kara":  {
              "Eyy": "00000000",
              "Myy": "11111111"
              }
            },
            {
            "Memphis":  {
              "Jyy": "22222222222222",
              "Tyy": "33"
              }
            },
            {
            "Dorothy":  {
              "Vyy": "999999999",
              "Qyy": "7777777777"
              }
            }
          ]
        },
        {
        "GreenTea":  ["111111", "2222222", "333333", "444444"]
        },
        {
        "Beer":  {
          "EEE": "Zyy",
          "JJJ": "Ayy"
          }
        }
      ],
    "VV": "BBBB"
    },
  "postalCode": "630-0192"
  },
"aa":  {
  "bb": "bb11",
  "cX":  ["cc11", "cc22", "cc33", "cc44"],
  "dd": "dd11",
  "dx":  [
      {
      "d1":  {
        "d2":  {
          "d3":  {
            "d4": "fufufufuf"
            }
          }
        }
      },
      {
      "dn1":  {
        "dn2":  {
          "dn3":  {
            "dn4": "jujuju"
            }
          }
        }
      }
    ]
  },
"phoneNumbers":  [
    {
    "type": "iPhone",
    "number": "0123-4567-8888"
    },
    {
    "type": "home",
    "number": "0123-4567-8910"
    },
    {
    "type": "company",
    "teams":  [
        {
        "hr": "111-2222"
        },
        {
        "rnd": "2222-3333"
        },
        {
        "pm": "333-4444"
        }
      ]
    }
  ]
}

Succeeded 0

[JSON] delete 0 address.XX.YY
Succeeded 0

[JSON] print 0

{
"firstName": "John",
"lastName": "doe",
"age": "26",
"address":  {
  "streetAddress": "naist street",
  "city": "Nara",
  "hello":  {
    "mygod":  {
      "vaccine":  {
        "job":  {
          "EEEE": "VAC"
          }
        }
      }
    },
  "XX":  {
    "PP":  ["HELLO", "MISTER", "KEY"],
    "ZZ":  ["11111", "22222", "33333", 
        {
        "BEEF":  {
          "STEAK": "WELLDONE"
          }
        }
      ],
    "VV": "BBBB"
    },
  "postalCode": "630-0192"
  },
"aa":  {
  "bb": "bb11",
  "cX":  ["cc11", "cc22", "cc33", "cc44"],
  "dd": "dd11",
  "dx":  [
      {
      "d1":  {
        "d2":  {
          "d3":  {
            "d4": "fufufufuf"
            }
          }
        }
      },
      {
      "dn1":  {
        "dn2":  {
          "dn3":  {
            "dn4": "jujuju"
            }
          }
        }
      }
    ]
  },
"phoneNumbers":  [
    {
    "type": "iPhone",
    "number": "0123-4567-8888"
    },
    {
    "type": "home",
    "number": "0123-4567-8910"
    },
    {
    "type": "company",
    "teams":  [
        {
        "hr": "111-2222"
        },
        {
        "rnd": "2222-3333"
        },
        {
        "pm": "333-4444"
        }
      ]
    }
  ]
}

Succeeded 0
[JSON] 

  ```

