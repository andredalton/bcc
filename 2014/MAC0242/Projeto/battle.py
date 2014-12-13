# Para manipular a linha de comando
import os, sys, getopt, re

# Para manipular e validar arquivos XML
from lxml import etree
from lxml.etree import XMLSyntaxError, Element

# Quem é este pokemon?
from pokemon.pokemon import Pokemon

# Battle state schema file
bss = 'battle_state.xsd'

def usage():
    """Imprime instruções de uso do programa."""
    uso = """
Este programa carrega um pokemon para uso como cliente ou servidor.

    -h --help       Imprime isto
    -a --auto       Inicializa com o pokemon em modo automatico
    -f --file       Carrega as informações de um arquivo separado por linhas.
    -x --xml        Carrega as informações de um arquivo xml.
    -p --port       Permite a passagem da porta de acesso do servidor por linha de comando
    -H --host       Permite a passagem da URL principal do programa por linha de comando
    """
    print(uso)

def command_line(argv, host="0.0.0.0", port=5000):
    """ Faz o tratamento da entrada de dados. """
    pk = Pokemon()
    flag = False
    p = port
    h = host
    
    if len(argv) > 0:
        try:
            opts, args = getopt.getopt(argv, "haf:x:p:H:", ["help", "auto", "file=", "xml=", "port=", "host="])
        except getopt.GetoptError as err:
            print(err)
            usage()
            sys.exit(2)
        # Evitando erros com passagens de argumentos com conteudo obrigatorio vazio.
        args = ["-h", "--help", "-a", "--auto", "-f", "--file", "-x", "--xml", "-p", "--port", "-H", "--host"]
        for o, a in opts:
            if o in ("-h", "--help"):
                usage()
                sys.exit()
            elif o in ("-a", "--auto"):
                pk.set_auto()
            elif o in ("-f", "--file"):
                if a in args:
                    print("option -f requires argument")
                    usage()
                    sys.exit()
                try:
                    f = open(a, "r")
                except FileNotFoundError:
                    print("Arquivo nao encontrado!")
                    sys.exit()
                pk.load(f)
                flag = True
            elif o in ("-x", "--xml"):
                if a in args:
                    print("option -x requires argument")
                    usage()
                    sys.exit()
                try:
                    s = open(a, "r").read()
                except FileNotFoundError:
                    print("Arquivo nao encontrado!")
                    sys.exit()
                try:
                    pk.load_xml(validate(s)[0])
                except TypeError:
                    sys.exit()
                flag = True
            elif o in ("-p", "--port"):
                if a in args:
                    print("option -p requires argument")
                    usage()
                    sys.exit()
                try:
                    p = int(a)
                except ValueError:
                    print("Por favor passe uma porta valida!")
                    sys.exit()
            elif o in ("-H", "--host"):
                if a in args:
                    print("option -H requires argument")
                    usage()
                    sys.exit()
                h = a
            else:
                assert False, "opcao nao tratada"
    else:
        pk.load()
    if flag:    
        return (pk, p, h)
    return None

def validate(s):
    """ Faz a validação de um battle_state. """
    bsv = open(bss, "r").read()
    xml = re.sub("encoding=['\"].*['\"]", "", s)
    schema_root = etree.XML(bsv)
    schema = etree.XMLSchema(schema_root)
    parser = etree.XMLParser(schema = schema)
    try:
        root = etree.fromstring(xml, parser)
    except XMLSyntaxError:
        print("Formato XML incorreto!")
        return None
    return root.findall("pokemon")

def make_battle_state(pk1, pk2=None):
    """ Gera um battle_state. """
    top = Element('battle_state')
    x1 = pk1.make_xml()
    top.append(x1)
    if pk2 is not None:
        x2 = pk2.make_xml()
        top.append(x2)
    return etree.tostring(top, xml_declaration=True, pretty_print=True, encoding="UTF-8").decode("utf-8")

def simple_duel(patt, pdef, n=None, run=True, ppm=None):
    """ Resolve um duelo simples. 

    patt: Pokemon atacante
    pdef: Pokemon defensor
    n:    Número do ataque
    run:  Para realizar o ataque
    ppm:  Memória dos pps, usado quando o usuário está comandando o pokemon
    """
    an = None
    if patt.get_HP() > 0 and pdef.get_HP() > 0:
        params = {
            "name1":patt.get_name(),
            "name2":pdef.get_name(),
            "hp1":patt.get_HP(),
            "hp2":pdef.get_HP(),
        }
        print("\n%(hp1)d\t- %(name1)s" % params)
        print("%(hp2)s\t- %(name2)s\n" % params)
        if patt.get_auto():
            an = patt.on_my_own(pdef)
            a = patt.select_attack(an)
        elif n is not None:
            an = int(n)
            a = patt.select_attack(an)
        else:
            a = None
            if patt.left_pp() > 0 and not patt.get_auto():
                print("\nAtaques de", patt.get_name())
                patt.print_attack(ppm)
                while a is None:
                    try:
                        an = int(input("Selecione um ataque para " + patt.get_name() + ": "))
                        a = patt.select_attack(an)
                    except ValueError:
                        print("Digite um número entre 1 e", patt.get_nattack())
                    if a is None:
                        print("Digite um número entre 1 e", patt.get_nattack())
            else:
                print("%(name)s has no moves left!" % {"name": patt.get_name()})
                an = 0
                a = patt.select_attack(0)
        if run:
            a.prepare(pdef)
            a.action()
            if pdef.get_HP() == 0:
                print("%s fainted!" % pdef.get_name())
            if patt.get_HP() == 0:
                print("%s fainted!" % patt.get_name())
            if pdef.get_HP()==0 or patt.get_HP()==0:
                print("\nBatalha encerrada!")
    return an