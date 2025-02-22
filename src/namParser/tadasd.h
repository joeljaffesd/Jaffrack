#ifndef DSP_DATA_H
#define DSP_DATA_H

#include <vector>
#include <string>

struct dspData {
    std::string version = "0.5.4";
    std::string architecture = "WaveNet";
    std::string config = R"({"layers": [{"input_size": 1, "condition_size": 1, "head_size": 2, "channels": 4, "kernel_size": 3, "dilations": [1, 2, 4, 8, 16, 32, 64], "activation": "Tanh", "gated": false, "head_bias": false}, {"input_size": 4, "condition_size": 1, "head_size": 1, "channels": 2, "kernel_size": 3, "dilations": [128, 256, 512, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512], "activation": "Tanh", "gated": false, "head_bias": true}], "head": null, "head_scale": 0.02})";
    std::string metadata = R"({"date": {"year": 2024, "month": 11, "day": 24, "hour": 22, "minute": 9, "second": 34}, "loudness": -19.41183853149414, "gain": 0.7782802589111386, "training": {"settings": {"ignore_checks": false}, "data": {"latency": {"manual": null, "calibration": {"algorithm_version": 1, "delays": [0], "safety_factor": 1, "recommended": -1, "warnings": {"matches_lookahead": false, "disagreement_too_high": false}}}, "checks": {"version": 3, "passed": true}}, "validation_esr": 0.04797653622481905}})";
    std::vector<float> weights = {-0.3370203971862793, 0.32957449555397034, -1.1366111040115356, -0.6931918859481812, 0.3064546585083008, 0.5009207129478455, 0.09008336812257767, -0.07587206363677979, -0.3249204456806183, -0.01890733279287815, 0.1944701224565506, 0.2308705449104309, -0.1734888106584549, 0.09665849804878235, 0.17799507081508636, 0.11700441688299179, 0.3612372875213623, 0.5457345247268677, -0.364864319562912, -0.3335269093513489, -0.19628150761127472, 0.3349662721157074, 0.14027197659015656, 0.5134583115577698, -0.12964482605457306, 0.23146139085292816, 0.5636417865753174, -0.3395102620124817, -0.07208511233329773, 0.040410563349723816, -0.35150963068008423, 0.10365580022335052, -0.2761848270893097, 0.05283275619149208, -0.07301492244005203, -0.19558866322040558, -0.37105581164360046, 0.36228516697883606, 0.21881191432476044, -0.04484579339623451, -0.1351061463356018, -0.18028327822685242, -0.046186741441488266, -0.1366598904132843, -0.10964859277009964, -0.108574278652668, -0.044539015740156174, -0.020801033824682236, -0.1637088805437088, -0.17185574769973755, 0.08260473608970642, 0.25982531905174255, 0.13298171758651733, 0.05218786746263504, 0.2907980978488922, -0.09552787989377975, 0.08087475597858429, -0.7193799614906311, -0.5028576850891113, -0.48829224705696106, -0.008787781931459904, 0.1089107096195221, -0.19811823964118958, 0.25393280386924744, 0.671694278717041, 0.5481975674629211, 0.4052887260913849, 0.6047263741493225, 0.5689662098884583, 0.8454858064651489, -0.08297882229089737, 0.1944725066423416, 0.003545737359672785, 0.5064110159873962, 0.6419866681098938, 0.394057035446167, 0.15427853167057037, -0.3262452483177185, 0.0059689418412745, -0.16952435672283173, -0.6137609481811523, 0.15486344695091248, 0.5887894630432129, -0.5769950151443481, -0.1126023381948471, 0.09880761802196503, -0.4018574655056, -0.1629350483417511, 0.6006867289543152, -0.6037328839302063, -0.05056376755237579, 0.4821237325668335, 0.3890492022037506, 0.5005057454109192, -0.04001035913825035, -0.3241795003414154, -0.07272183895111084, 0.19439943134784698, -0.2307359129190445, 0.03921511769294739, 0.43230879306793213, 0.14405964314937592, 0.06799519062042236, -0.09202992171049118, -0.22096386551856995, 0.23515646159648895, -0.19826364517211914, -0.30258113145828247, 0.3548109233379364, -0.37372341752052307, -0.062198664993047714, 0.47480037808418274, -0.22548390924930573, 0.4499802589416504, 0.4248661994934082, -0.0700182318687439, 0.09707462042570114, -0.04795703664422035, -0.05726709961891174, 0.3332248330116272, 0.1654752939939499, 0.09322627633810043, 0.14947821199893951, 0.10145622491836548, -0.3450689911842346, 0.08476608991622925, -0.06866051256656647, -0.2023216187953949, -0.10898067057132721, -0.07668837904930115, -0.07912334054708481, 0.1045641303062439, -0.956230878829956, 0.37334099411964417, -0.15550656616687775, -0.07898203283548355, 0.021256575360894203, -0.015781933441758156, -0.37271735072135925, 0.3011936843395233, 0.8890138864517212, -0.10588755458593369, 0.2517126798629761, -0.036846719682216644, -0.9098713397979736, -0.42610597610473633, 0.7225896716117859, -0.12210538983345032, 0.4046488106250763, -0.6929494738578796, -1.202880859375, -0.6028474569320679, -0.02318308688700199, 0.3053351938724518, 0.2325262576341629, -0.3412976562976837, 0.2851601243019104, 0.011035781353712082, 0.02908194623887539, -0.13057193160057068, -0.014227401465177536, 0.43273603916168213, -0.14696809649467468, -0.20198139548301697, 0.17880263924598694, -0.3779793083667755, -0.23775574564933777, 0.2593577802181244, -0.16615574061870575, 0.20659925043582916, 0.1295776665210724, 0.10584680736064911, 0.8003136515617371, 0.3597930371761322, -0.9170529246330261, -0.006102817133069038, 0.3100798726081848, 0.09953609108924866, 0.44975030422210693, -0.3749929368495941, 0.1577215939760208, -0.04747654125094414, 0.6118245124816895, 0.29382842779159546, 0.47745251655578613, 0.6832981109619141, -0.31215018033981323, 0.05153777822852135, 0.08087035268545151, 0.3552759289741516, 0.29404905438423157, 0.5139506459236145, -0.46362411975860596, -0.04475856572389603, -0.4606945812702179, -0.44791460037231445, -0.20848046243190765, 0.6465319395065308, -0.20382505655288696, -0.1139063760638237, 0.19189953804016113, -0.16966946423053741, -0.20452626049518585, 0.046107757836580276, -0.16805793344974518, 0.051951125264167786, 0.2302531749010086, 0.019266780465841293, 0.42958593368530273, 0.2700434625148773, -0.5684868693351746, -0.1420949101448059, 0.07800167798995972, -1.8098007440567017, -0.04388900101184845, 0.08549810945987701, -0.6526550054550171, -0.5873216390609741, 0.38584932684898376, -0.9311724901199341, 0.25332334637641907, 1.0680345296859741, 0.1634814590215683, 0.22395139932632446, -0.6686448454856873, -0.09709802269935608, -0.6278405785560608, -0.2008752077817917, -0.1675952523946762, -0.026553940027952194, -0.18257734179496765, 0.3525506556034088, 0.5250971913337708, 0.2661709189414978, 0.05659079551696777, -0.13147370517253876, 0.3478526473045349, 0.019581325352191925, -0.5137700438499451, -0.09264341741800308, 0.40181785821914673, 0.5733699798583984, -0.6724161505699158, -0.06398901343345642, -0.8723219633102417, -1.2212307453155518, 0.3771892189979553, -0.7983341813087463, -0.0578087642788887, 0.10802458971738815, 0.20417878031730652, 0.8330503702163696, 0.3207469582557678, 0.23830607533454895, -0.19417478144168854, -0.13794945180416107, -0.43452000617980957, -0.5505028963088989, -0.9868327975273132, -0.6252298951148987, -0.2146485298871994, 0.25483429431915283, -0.316573828458786, 0.16007843613624573, 0.7031489014625549, -0.013543540611863136, -0.14487013220787048, -0.23047052323818207, 0.7554811835289001, 0.9813602566719055, -0.13881723582744598, 0.17608419060707092, -0.34566283226013184, 0.37512192130088806, -0.067861407995224, -0.6267088651657104, 0.4153449237346649, 0.4055466055870056, 0.11433158069849014, 0.9407482743263245, 0.00037040383904241025, -0.2093692421913147, -0.14753186702728271, 0.09403050690889359, 0.6671385765075684, 0.6388497352600098, -0.11446782201528549, 0.5782512426376343, -0.6119665503501892, 0.057892344892024994, -0.927728533744812, -0.3563680350780487, -0.2486332654953003, 0.16112945973873138, 0.1825966238975525, -0.16615110635757446, 0.45675522089004517, -0.775650680065155, 0.6740313768386841, -0.5705321431159973, -0.02130044810473919, -0.3214634656906128, -0.8265049457550049, 0.08498480170965195, 0.11786165833473206, -0.226487934589386, -0.004602390341460705, 0.1395227313041687, 0.24233746528625488, 0.21258223056793213, -0.06304440647363663, 0.011257349513471127, -0.09478825330734253, 0.06516978144645691, 0.35750773549079895, 0.3412981629371643, -0.42129042744636536, 0.7264055013656616, 0.29342567920684814, -0.40443164110183716, 0.0558832548558712, -0.13246427476406097, 0.0031489210668951273, 0.1298532336950302, 0.00970356073230505, -0.025399358943104744, 0.2808711528778076, 0.18690429627895355, -0.04256707802414894, 0.11207272112369537, 0.5957072377204895, -0.3311727046966553, -0.17941001057624817, 0.3284751772880554, -0.020314175635576248, 0.38747963309288025, -0.16393116116523743, -0.42151734232902527, -0.03394722938537598, 0.6203565001487732, -0.3451811373233795, 0.12928268313407898, 0.10623021423816681, 0.5281867384910583, 0.34684452414512634, 0.2500978410243988, 0.7250744700431824, 0.4873397648334503, 0.09688115864992142, -0.317722886800766, 0.11995876580476761, -0.18112999200820923, -0.5295400619506836, -0.2989564836025238, -0.9654408693313599, 0.9102922677993774, -0.13658104836940765, -0.05995212122797966, -0.12992660701274872, 0.08026742935180664, -0.0006027384079061449, -0.47784188389778137, -0.7474272847175598, 0.716323733329773, -0.8346588611602783, 0.2807300090789795, 0.6062312722206116, 0.9606195688247681, -1.0466190576553345, -1.1799288988113403, -0.6110858917236328, 0.34960854053497314, -0.8763059973716736, -0.09694400429725647, -0.5903207659721375, -1.3740155696868896, 0.14581066370010376, 0.21811050176620483, -0.6642947196960449, 0.7375636696815491, 0.12922967970371246, 0.022626696154475212, 0.12789693474769592, 0.25267815589904785, -0.1557934284210205, -0.3634452521800995, 0.7456403970718384, 0.5907555222511292, 0.9963156580924988, 0.19428925216197968, -0.12803415954113007, -0.19207200407981873, -0.5787118673324585, -0.22936810553073883, -0.40284013748168945, 0.2260396033525467, 0.046317193657159805, -0.15947647392749786, -0.186836838722229, -0.08037123829126358, 0.48081064224243164, 0.09321098029613495, -0.16892589628696442, 0.16206835210323334, -0.0805547758936882, -0.4744276702404022, 0.2477491796016693, 0.11807291954755783, -0.202090784907341, -0.13516634702682495, -0.11185405403375626, 0.0849640741944313, -0.017482031136751175, 0.2817203104496002, -0.0546993725001812, -0.2699790596961975, -0.03169092535972595, 0.2117186188697815, -0.13265204429626465, 0.2433658242225647, -0.2233041673898697, 0.815659761428833, 0.018589267507195473, 1.6449368000030518, 0.45892882347106934, -0.699068546295166, 0.4078717529773712, -1.2844130992889404, -0.10876280814409256, 0.04965146631002426, 0.7048719525337219, -0.21550878882408142, 0.35053038597106934, 0.052812401205301285, 0.012586870230734348, 0.00022250307665672153, 1.3180232048034668, -0.5901572704315186, 0.6401488780975342, -0.3958008885383606, -0.2956216335296631, 0.4820288121700287, 0.18573825061321259, 0.2834623157978058, -0.40554219484329224, -0.20647366344928741, -0.8938111066818237, 0.197719544172287, -0.6395142674446106, -0.682780385017395, 0.46827244758605957, 0.6040794253349304, 1.32100248336792, 0.8697410225868225, -0.20475828647613525, 0.5358237028121948, -0.09711135923862457, 0.274150013923645, -0.35139405727386475, -0.3392947018146515, 0.0010460582561790943, -0.12492477893829346, -0.07941209524869919, -0.13590720295906067, -0.17334593832492828, 0.6881518363952637, -0.053798675537109375, 0.026257526129484177, -0.8476612567901611, -0.07880750298500061, 0.19640779495239258, 1.1782892942428589, 0.11830028146505356, 0.4959532618522644, -0.014663999900221825, -0.12932267785072327, -0.6447799801826477, 0.06951653957366943, -0.07105308771133423, 0.3361857533454895, 0.07310333102941513, -0.33362773060798645, -0.9996950626373291, 0.11866084486246109, -0.11467375606298447, -0.053386904299259186, 0.01831885427236557, -0.3087538778781891, 0.1300387680530548, 0.02095138095319271, 0.5717374682426453, 0.03406907990574837, 0.013331406749784946, -0.4624088704586029, 0.10257925093173981, 0.004099054262042046, 0.38839051127433777, 0.37526193261146545, -0.03505793586373329, -0.2559376060962677, -0.3986695408821106, 0.22030054032802582, 0.23526246845722198, 0.5166817903518677, -0.09090712666511536, -0.15481731295585632, 0.22429347038269043, -0.08416879177093506, -0.09394889324903488, 0.17961479723453522, 0.02858671545982361, 0.16175244748592377, -0.8501423001289368, -0.29519107937812805, 0.4560532569885254, 1.4141299724578857, -0.7016439437866211, -0.12904419004917145, 0.005043901968747377, 0.6725428104400635, 0.6640978455543518, 0.1644316464662552, 0.060429684817790985, -0.5214281678199768, -0.7468130588531494, -0.7427052855491638, 0.6490454077720642, 0.017072666436433792, 0.6577188968658447, -0.29788440465927124, 1.1530942916870117, 1.2961347103118896, -0.5901926755905151, -0.31691044569015503, -0.4505183696746826, 0.16767902672290802, -0.2279488891363144, -0.05195002257823944, -0.1796751767396927, 0.000548715062905103, -0.13876552879810333, -0.17172889411449432, -0.07857327163219452, -0.11936791986227036, -0.1051853820681572, 0.10757352411746979, -0.10305708646774292, 0.026843931525945663, -0.08060397207736969, 0.10954759269952774, -0.17946487665176392, -0.17940106987953186, 0.14644303917884827, -0.24458098411560059, -0.016809755936264992, -0.06513844430446625, -0.15858590602874756, -0.33352020382881165, 0.06985298544168472, -0.4422807991504669, -0.3727388083934784, -0.00852165836840868, -0.1300082802772522, 0.14866039156913757, 0.07210998237133026, 0.1507558375597, 0.08587907999753952, 0.11886976659297943, 0.2288994938135147, 0.35080772638320923, 1.405548334121704, 0.5716220736503601, -0.16204865276813507, -0.38596558570861816, 0.2431691735982895, 0.3639453053474426, 0.057655349373817444, -0.5751202702522278, 0.25744298100471497, 0.9361875653266907, 0.14757320284843445, 0.08609224855899811, -0.08950051665306091, -0.3406858444213867, 0.34003087878227234, -0.47873401641845703, 0.17695645987987518, -0.12012013792991638, 0.20662780106067657, 0.5672490000724792, -0.2661049962043762, 0.44392672181129456, -1.0289945602416992, 1.6360154151916504, 0.2214267998933792, 0.2664717435836792, -0.0858510285615921, 0.4277343451976776, 0.14461302757263184, -0.5875172019004822, 0.23923711478710175, 0.6460019946098328, -0.016186874359846115, 0.2578495442867279, -0.22273774445056915, -0.12872210144996643, -0.013872762210667133, -0.2531580626964569, -0.051501307636499405, -0.031606897711753845, 0.07274042814970016, -0.7258496880531311, 0.0003468695795163512, 0.12176354974508286, -0.06649007648229599, 0.4561936855316162, 0.18207024037837982, 0.03576131537556648, 0.4053055942058563, 0.39243945479393005, -0.7916755080223083, 0.04480283334851265, 0.031192241236567497, -0.08087056875228882, 0.006401962134987116, 0.19467899203300476, 0.08459486812353134, -0.21913854777812958, -0.19230639934539795, 0.7863292098045349, -0.05308495834469795, -0.07928190380334854, 0.11965063959360123, 0.26995527744293213, 0.8779568672180176, 0.12834343314170837, 0.03031996265053749, -0.921718955039978, -0.39560434222221375, 0.29802465438842773, -0.2377125471830368, 0.07465445250272751, -0.30875280499458313, -0.6331236362457275, -0.5024558901786804, 0.5932109951972961, 0.22337006032466888, 0.17329806089401245, -0.474651575088501, -0.06588359922170639, 0.00841756071895361, -0.2530364692211151, 0.21332696080207825, 0.09274909645318985, -0.2763003706932068, 0.02895006351172924, 0.2321842759847641, 1.4702396392822266, -0.8064046502113342, 0.03238401934504509, 0.03958937153220177, -0.19188115000724792, 0.230317622423172, -0.6295995116233826, -0.14519988000392914, 0.31991150975227356, -0.07379680871963501, -0.43365269899368286, 0.2699199914932251, -0.28499338030815125, -0.3248133659362793, -0.32751724123954773, -0.06739804148674011, 0.06083657220005989, -0.0010861532064154744, 0.005539214704185724, 0.01649026945233345, -0.006674904376268387, 1.0198298692703247, 0.6476758718490601, 1.0085642337799072, -0.9626359939575195, -0.03382312133908272, 0.07426963746547699, 0.5265522003173828, 0.10241907835006714, 0.071794293820858, -0.41702014207839966, 0.32664820551872253, -0.0042631723918020725, -0.08306172490119934, -1.601311206817627, 0.49250197410583496, 0.28940534591674805, 0.4633338749408722, 0.011015231721103191, 0.06760242581367493, -0.13124319911003113, 0.004679699428379536, 0.3163744807243347, -1.1028271913528442, 0.014600828289985657, -0.8631594777107239, -0.16048087179660797, -0.23689502477645874, 0.009111180901527405, 0.45596763491630554, 0.8137784004211426, 0.43023887276649475, -0.2790876626968384, 0.5050454139709473, 0.07611642777919769, -0.11536887288093567, -0.4430871605873108, -0.3751923441886902, 0.38143402338027954, 0.40649354457855225, -0.2718594968318939, 0.509033739566803, -0.20809872448444366, 0.16203290224075317, -0.06801142543554306, -2.176072359085083, -0.10186723619699478, 0.7834981679916382, 1.3943822383880615, 0.495033860206604, -0.3209370970726013, -0.6112162470817566, 0.26179254055023193, -0.6743008494377136, 0.4139299988746643, -0.991719663143158, -0.2676549553871155, 0.16153913736343384, 0.11493129283189774, 0.5776709914207458, -0.2921742796897888, 0.4719140827655792, -0.03423723205924034, 0.15719600021839142, -0.11941937357187271, 0.31772753596305847, 0.05928804725408554, -0.15173718333244324, 1.214691400527954, -0.6022873520851135, 0.2225070297718048, -0.22764748334884644, 0.15677645802497864, -0.43534013628959656, 0.6058639287948608, -0.7986347675323486, 0.20621290802955627, -0.4048105478286743, -0.22640171647071838, 0.37914425134658813, -0.039517417550086975, 0.24673870205879211, -0.3737056255340576, -0.35729116201400757, -0.007113681174814701, -0.016093293204903603, 0.07540252059698105, -1.0156875848770142, 0.26849353313446045, 0.14139120280742645, 1.9782825708389282, -0.7866871953010559, -0.4722948670387268, 0.26383906602859497, -0.5461293458938599, 0.3476671278476715, 0.18240293860435486, -0.4297011196613312, -0.5038434267044067, 0.13339094817638397, 0.04629331827163696, -0.2296959012746811, 0.5950871706008911, 0.18313048779964447, 0.45957469940185547, -0.7065386772155762, -0.2558080554008484, -0.1778911054134369, -0.30873262882232666, -0.31149452924728394, -0.2775780260562897, 0.0660097524523735, -0.8352035880088806, -0.7164503931999207, 0.19113114476203918, 0.523564338684082, 0.2060297280550003, 0.00877835601568222, 0.17997004091739655, -0.6404172778129578, -0.1987057477235794, -0.5571845173835754, 0.08389665931463242, 0.3706851005554199, -0.4826377332210541, 0.740685760974884, -0.6713391542434692, 0.3488774299621582, 0.02405058965086937, 0.14648549258708954, -0.5135130286216736, -0.3673512935638428, 0.5287421345710754, -0.9364384412765503, -0.14685793220996857, 1.4614187479019165, -1.316295862197876, -0.25133925676345825, 0.08686210215091705, -0.223926842212677, 0.301725834608078, -0.4908387362957001, -0.04472803324460983, -0.11234064400196075, -0.11386106163263321, 0.0050812955014407635, 0.22660377621650696, 0.7022904753684998, 0.21376179158687592, -0.024073945358395576, 0.18616831302642822, 0.1309565156698227, -0.29862186312675476, 0.6211906671524048, 0.36170440912246704, 0.07090878486633301, -0.2630368769168854, -0.5921692252159119, 0.2861429452896118, 0.09047992527484894, -0.2831847667694092, -1.6569209098815918, 2.1919288635253906, -0.2556544840335846, 0.019999999552965164};
    float expected_sample_rate = 48000;
};

#endif // DSP_DATA_H
