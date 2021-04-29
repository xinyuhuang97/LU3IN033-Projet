from typing import List
import sys
from collections import defaultdict

Type_Eth_data={"0800":'IPv4', "0805":'X.25 niveau 3', "0806":'ARP', "8035":'RARP', "8098":'Appletalk'}
Type_IP_option={"0":'End of Options', "1":'No Operation', "7":'Record Route', "68":'Time Stamp', "131":'Loose Routing', "137":'Strict Routing'}
Type_IP_protocol={"01":'ICMP', "02":'IGMP', "06":'TCP', "08":'EGP', "09":'IGP', "17":'UDP', "36":'XTP', "46":'RSVP'}
Type_TCP_option={"00":'End of Option List', "01":'No-Operation', "02": 'Maximum Segment Size', "03": 'WSOPT - Window Scale', "04": 'SACK Permitted', "05": 'SACK(Selective ACK)', "06": 'Echo', "07":'Echo Reply', "08":'Timestamps', "09":' Partial Order Connection Permitted', "0A":'Partial Order Service Profile', "0B":'CC', "0C": 'CC.NEW', "0D": 'CC.ECHO', "0E": 'TCP Alternate Checksum Request', "OF":'TCP Alternate Checksum Data'}

def lire_fichier(file: str):
    with open(file) as f:
        temp = [line.split() for line in f.read().splitlines()]
    lg_avant=0
    final=[[]]
    offset_avant="0x00"
    l=0
    nb=0
    for line in temp:
        # si la premiere case est offset
        # si la ligne est complet
        if len(line[0])>2 and int(line[0], 16)==int(lg_avant)+ int(offset_avant,16):
            offset_avant=line[0]
            lg_avant=len(line)-1
            final[nb].append(line)
        else:
            if int(line[0],16)==0 and offset_avant!="0x00" :
                nb+=1
                offset_avant="0x00"
                lg_avant=len(line)-1
                final.append([])
                final[nb].append(line)
            else:
                if(len(line[0])<=2):
                    print("Erreur de Syntaxe: Fichier contenant ligne sans OFFSET")
                print("\tLigne imcomplet :",l)
        l+=1
    return final

def hexa_en_binaire(hex:str, nb_bit=0):
    val=int(hex, 16)
    if val==0:
        return [0, 0, 0, 0]
    fois=1
    reste=0
    puissant=0
    while val>=2**puissant:
        puissant+=1
    puissant-=1
    if nb_bit!=0:
        list=[0 for i in range(nb_bit+1)]
    else:
        list=[0 for i in range(puissant+1)]
    while val>0:
        if val>=2**puissant:
            val-=2**puissant
            list[puissant]=1
        puissant-=1
    return [str(x) for x in list[::-1]]

def obtenir_octets(donnee: List[List[str]], l : int, c : int, nb : int):
    list=[]
    while nb>0:
        if c+nb> len(donnee[l]):
            list=donnee[l][c:]
            nb=nb-(len(donnee[l])-c)
            c=1
            l+=1
        else:
            list2=donnee[l][c:c+nb]
            c=c+nb
            nb=0
    list_final=list + list2
    return (l, c, list_final)

def calcul_hex_en_valeur_decimal(hex: str):
    hex=''.join(hex)
    return sum([int(hex[i], 16)*16**(len(hex)-i-1) for i in range(len(hex))])

def annalyser(file: str):
    d=lire_fichier(file)
    cpt=0
    print("Nombre de Trames:", len(d))
    while(cpt<len(d)):
        print("-----------------------------------"+str(cpt+1)+"------------------------------------")
        print("Analyse de No", cpt+1, "trame")
        donnee=d[cpt]
        #print(donnee)
        l=0 #ligne
        c=1 #colonne
        "===================================ETHERNET================================="
        print("===============================ETHERNET===============================")
        result=obtenir_octets(donnee, l, c, 6)
        l, c, addresse_dest=(result[0], result[1], ':'.join(result[2]))
        print("Destination:", addresse_dest)
        result=obtenir_octets(donnee, l, c, 6)
        l, c, addresse_src=result[0], result[1], ':'.join(result[2])
        print("Source:", addresse_src)
        result=obtenir_octets(donnee, l, c, 2)
        l, c, type=result[0], result[1], ''.join(result[2])
        print("Type:", Type_Eth_data[type], "(0x"+type+")")
        "===================================IP================================="
        print("===================================IP=================================")
        result=obtenir_octets(donnee, l, c, 1)
        l, c, s=(result[0], result[1],result[2])
        version, IHL= str(s[0][0]), str(s[0][1])
        print("".join(hexa_en_binaire(version)),".... = Version:", version)
        print(".... "+"".join(hexa_en_binaire(IHL)),"Header Length:", int(IHL)*4, "bytes ("+ IHL+")")
        if(int(IHL)*4>20):
            ip_option=1
        else:
            ip_option=0
        result=obtenir_octets(donnee, l, c, 1)
        l, c, dsf=result[0], result[1], ':'.join(result[2])
        print("Differentiated Services Field: 0x"+dsf)
        #Differentiated services codepoint:
        #Explicit congestion notification:
        result=obtenir_octets(donnee, l, c, 2)
        l, c, leng=(result[0], result[1],result[2])
        length=calcul_hex_en_valeur_decimal(leng)
        print("Total Length:",length)
        result=obtenir_octets(donnee, l, c, 2)
        l, c, id=(result[0], result[1],result[2])
        val_id=calcul_hex_en_valeur_decimal(id)
        print("Identification: 0x"+''.join(id),"("+str(val_id)+")")
        result=obtenir_octets(donnee, l, c, 2)
        l, c, flag_info=(result[0], result[1],result[2])
        bin_flag=hexa_en_binaire(''.join(flag_info[1:]))
        print("Flags: 0x"+''.join(flag_info))
        print("Fragment offset:")
        result=obtenir_octets(donnee, l, c, 1)
        l, c, ttl=(result[0], result[1],result[2])
        print("Time to live:", calcul_hex_en_valeur_decimal(ttl))
        result=obtenir_octets(donnee, l, c, 1)
        l, c, protocol=(result[0], result[1],result[2])
        print("Protocol: "+Type_IP_protocol[''.join(protocol)]+" ("+str(calcul_hex_en_valeur_decimal(protocol))+")")

        TCP=0
        if int(''.join(protocol), 16)==6:
            TCP=1

        result=obtenir_octets(donnee, l, c, 2)
        l, c, header_cheksum=(result[0], result[1],result[2])
        print("Header checksum: 0x"+''.join(header_cheksum))
        result=obtenir_octets(donnee, l, c, 4)
        l, c, src_addresse=(result[0], result[1],result[2])
        print("Source: "+ '.'.join([str(int(str(i),16)) for i in src_addresse]))
        result=obtenir_octets(donnee, l, c, 4)
        l, c, dst_addresse=(result[0], result[1],result[2])
        print("Destination: "+ '.'.join([str(int(str(i),16)) for i in dst_addresse]))
        if ip_option==1:
            nb_bit=int(IHL)*4-20
            l, c, proto=(result[0], result[1],''.join(result[2]))
            print("Options: ("+ str(nb_bit)+" bytes)")
            result=obtenir_octets(donnee, l, c, 1)
            s=""
            kind_option=[]
            while nb_bit>0:
                result=obtenir_octets(donnee, l, c, 2)
                l, c, opt=(result[0], result[1],result[2])
                type, length=opt[0], opt[1]
                #print(s, Type_IP_option[type])
                Type=Type_IP_option.setdefault(type)
                s+="IP Option - "+str(Type)+"\n"
                kind_option.append(Type)
                s+="\tType: "+ type + "\n"
                s+="\tLength: "+ length +"\n"
                result=obtenir_octets(donnee, l, c, int(length,16))
                nb_bit-=int(length,16)
                l, c, val=(result[0], result[1],''.join(result[2]))
                s+="\tValue: "+val+"\n"
            s=s.rstrip('\r\n')
            print(s)
        if TCP==1:
            "===================================TCP================================="
            print("===================================TCP=================================")
            result=obtenir_octets(donnee, l, c, 2)
            l, c, src_port=(result[0], result[1],result[2])
            print("Source Port:", calcul_hex_en_valeur_decimal(src_port))
            result=obtenir_octets(donnee, l, c, 2)
            l, c, dst_port=(result[0], result[1],result[2])
            print("Destination Port:", calcul_hex_en_valeur_decimal(dst_port))
            result=obtenir_octets(donnee, l, c, 4)
            l, c, seq_num=(result[0], result[1],result[2])
            print("Sequence number:", calcul_hex_en_valeur_decimal(seq_num))
            result=obtenir_octets(donnee, l, c, 4)
            l, c, ack_num=(result[0], result[1],result[2])
            print("Sequence number:", calcul_hex_en_valeur_decimal(ack_num))
            result=obtenir_octets(donnee, l, c, 1)
            l, c, offset=(result[0], result[1],result[2])
            os=hexa_en_binaire(offset[0][0])
            print(''.join(os)+" ....= Header Length:",int(offset[0][0])*4,"("+offset[0][0]+")" )
            nb_octets=int(offset[0][0])*4
            res=hexa_en_binaire(offset[0][1])
            result=obtenir_octets(donnee, l, c, 1)
            l, c, flags_hex=(result[0], result[1],result[2])
            flags=res + hexa_en_binaire(''.join(flags_hex), 7)
            f=[]
            if int(flags[6])==1:
                urg=1
                f.append("URG")
            if int(flags[7])==1:
                ack=1
                f.append("ACK")
            if int(flags[8])==1:
                psh=1
                f.append("PSH")
            if int(flags[9])==1:
                rst=1
                f.append("RST")
            if int(flags[10])==1:
                syn=1
                f.append("SYN")
            if int(flags[11])==1:
                fin=1
                f.append("FIN")
            print("Flags: 0x"+ ''.join(flags_hex)+" "+"("+', '.join(f)+")")#+flags)#,"\n", flags)
            result=obtenir_octets(donnee, l, c, 2)
            l, c, window=(result[0], result[1],result[2])
            print("Window size value:",calcul_hex_en_valeur_decimal(window))
            result=obtenir_octets(donnee, l, c, 2)
            l, c, checksum2=(result[0], result[1],result[2])
            print("Checksum: 0x"+"".join(checksum2))
            result=obtenir_octets(donnee, l, c, 2)
            l, c, urgent_point=(result[0], result[1],result[2])
            print("Urgent pointer: "+ str(calcul_hex_en_valeur_decimal(urgent_point)))
            mtn=20
            if(mtn<nb_octets):
                string=""
                while(mtn<nb_octets):
                    result=obtenir_octets(donnee, l, c, 1)
                    mtn+=1
                    l, c, type=(result[0], result[1],''.join(result[2]))
                    string+="TCP Option - "+ Type_TCP_option[type]
                    if type=="08":
                        result=obtenir_octets(donnee, l, c, 9)
                        mtn+=9
                        l, c, size_value=(result[0], result[1],result[2])
                        TSV=size_value[1:6]
                        TERV=size_value[6:9]
                        string+= "Tsval:"+str(calcul_hex_en_valeur_decimal(''.join(TSV))) + " TSval:"+str(calcul_hex_en_valeur_decimal(''.join(TERV)))+"\n"
                        string+="\tKind:"+Type_TCP_option[type]+"("+str(calcul_hex_en_valeur_decimal(''.join(type)))+")\n"
                        string+="\tLength:"+ str(calcul_hex_en_valeur_decimal(''.join(size_value[0])))+"\n"
                        string+="\tTimestamp value:"+str(calcul_hex_en_valeur_decimal(''.join(TSV)))+"\n"
                        string+="\tTimestamp echo reply:"+str(calcul_hex_en_valeur_decimal(''.join(TERV)))+"\n"
                    elif (int(type, 16)<=1):
                        if int(type ,16)==1:
                            string+="(NOP)" +"\n\tKind:"+Type_TCP_option[type]+"("+str(int(type, 16)) +")"+"\n"
                        else:
                            string+="(EOP)" +"\n\tKind:"+Type_TCP_option[type]+"("+str(int(type, 16)) +")"+"\n"
                    else:
                        result=obtenir_octets(donnee, l, c, 1)
                        mtn+=1
                        l, c, size=(result[0], result[1],result[2])
                        string+="\n"+"\tKind:"+Type_TCP_option[type]+"("+str(calcul_hex_en_valeur_decimal(''.join(type))) + ")" + "\n"
                        size=calcul_hex_en_valeur_decimal(''.join(size))
                        result=obtenir_octets(donnee, l, c, 1)
                        l, c, vl=(result[0], result[1],''.join(result[2]))
                        string+="Value:"+vl
            string=string.rstrip('\r\n')
            print(string)
            "===================================HTTP================================="
            print("===================================HTTP=================================")
            http=""
            entete=""
            while l <len(donnee):
                while c < len(donnee[l]):
                    result=obtenir_octets(donnee, l, c, 1)
                    l, c, vl=(result[0], result[1],''.join(result[2]))
                    entete+=bytes.fromhex(vl).decode('utf-8')
                    if vl=="0a":
                        http+=entete
                        entete=''
                        break
                if c==len(donnee[l]):
                    c=1
                    l+=1
            http=http.rstrip('\r\n')
            print(http)
        else:
            print("----------------------Protocol not TCP, stop reading--------------------")
        print("---------------------------------Fin "+str(cpt+1)+"----------------------------------")
        cpt+=1

annalyser("./"+sys.argv[1])
