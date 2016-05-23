var dayNames = ["Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"];

var filmesNames = ["Pai de Família", "Bob Esponja", "Cidade de Deus", "7 x 1"];

var sinopses = [
    "<p>Podemos já vislumbrar o modo pelo qual a execução dos pontos do programa agrega valor ao estabelecimento do investimento em reciclagem técnica. O empenho em analisar a mobilidade dos capitais internacionais apresenta tendências no sentido de aprovar a manutenção dos modos de operação convencionais. No mundo atual, a crescente influência da mídia talvez venha a ressaltar a relatividade dos níveis de motivação departamental. </p><p>O incentivo ao avanço tecnológico, assim como a estrutura atual da organização pode nos levar a considerar a reestruturação dos índices pretendidos. Não obstante, a adoção de políticas descentralizadoras cumpre um papel essencial na formulação do impacto na agilidade decisória. Por conseguinte, o fenômeno da Internet causa impacto indireto na reavaliação do orçamento setorial. A nível organizacional, o desenvolvimento contínuo de distintas formas de atuação ainda não demonstrou convincentemente que vai participar na mudança da gestão inovadora da qual fazemos parte. Desta maneira, a consulta aos diversos militantes deve passar por modificações independentemente das formas de ação. </p><p>Acima de tudo, é fundamental ressaltar que o desafiador cenário globalizado possibilita uma melhor visão global dos relacionamentos verticais entre as hierarquias. A prática cotidiana prova que a complexidade dos estudos efetuados afeta positivamente a correta previsão das novas proposições. Pensando mais a longo prazo, o consenso sobre a necessidade de qualificação assume importantes posições no estabelecimento dos métodos utilizados na avaliação de resultados. Ainda assim, existem dúvidas a respeito de como o julgamento imparcial das eventualidades auxilia a preparação e a composição dos paradigmas corporativos. </p>",
    "<p>Não obstante, a constante divulgação das informações ainda não demonstrou convincentemente que vai participar na mudança do orçamento setorial. Evidentemente, a determinação clara de objetivos estimula a padronização das direções preferenciais no sentido do progresso. O que temos que ter sempre em mente é que o comprometimento entre as equipes auxilia a preparação e a composição do retorno esperado a longo prazo. </p><p>Todas estas questões, devidamente ponderadas, levantam dúvidas sobre se o aumento do diálogo entre os diferentes setores produtivos possibilita uma melhor visão global da gestão inovadora da qual fazemos parte. Do mesmo modo, o desenvolvimento contínuo de distintas formas de atuação cumpre um papel essencial na formulação das posturas dos órgãos dirigentes com relação às suas atribuições. No mundo atual, o fenômeno da Internet acarreta um processo de reformulação e modernização do impacto na agilidade decisória. Por conseguinte, a execução dos pontos do programa afeta positivamente a correta previsão das formas de ação. </p><p>As experiências acumuladas demonstram que o acompanhamento das preferências de consumo desafia a capacidade de equalização das diversas correntes de pensamento. Assim mesmo, o consenso sobre a necessidade de qualificação pode nos levar a considerar a reestruturação dos conhecimentos estratégicos para atingir a excelência. Gostaria de enfatizar que a crescente influência da mídia estende o alcance e a importância do sistema de formação de quadros que corresponde às necessidades. No entanto, não podemos esquecer que a complexidade dos estudos efetuados representa uma abertura para a melhoria do investimento em reciclagem técnica. </p>",
    "<p>É importante questionar o quanto a constante divulgação das informações ainda não demonstrou convincentemente que vai participar na mudança dos paradigmas corporativos. Do mesmo modo, a expansão dos mercados mundiais promove a alavancagem das direções preferenciais no sentido do progresso. O que temos que ter sempre em mente é que a consulta aos diversos militantes acarreta um processo de reformulação e modernização do retorno esperado a longo prazo. Todas estas questões, devidamente ponderadas, levantam dúvidas sobre se o desafiador cenário globalizado aponta para a melhoria das formas de ação. </p><p>No mundo atual, a execução dos pontos do programa cumpre um papel essencial na formulação das posturas dos órgãos dirigentes com relação às suas atribuições. Desta maneira, a percepção das dificuldades faz parte de um processo de gestão das diversas correntes de pensamento. A nível organizacional, o desenvolvimento contínuo de distintas formas de atuação nos obriga à análise das condições financeiras e administrativas exigidas. </p><p>A prática cotidiana prova que o acompanhamento das preferências de consumo desafia a capacidade de equalização do investimento em reciclagem técnica. Assim mesmo, o consenso sobre a necessidade de qualificação estimula a padronização da gestão inovadora da qual fazemos parte. É claro que o fenômeno da Internet estende o alcance e a importância do sistema de formação de quadros que corresponde às necessidades. </p>",
    "<p>Por outro lado, o fenômeno da Internet oferece uma interessante oportunidade para verificação das diretrizes de desenvolvimento para o futuro. É importante questionar o quanto o desafiador cenário globalizado pode nos levar a considerar a reestruturação do processo de comunicação como um todo. Acima de tudo, é fundamental ressaltar que a valorização de fatores subjetivos acarreta um processo de reformulação e modernização dos índices pretendidos. </p><p>O cuidado em identificar pontos críticos na determinação clara de objetivos estende o alcance e a importância dos procedimentos normalmente adotados. No mundo atual, o comprometimento entre as equipes apresenta tendências no sentido de aprovar a manutenção das posturas dos órgãos dirigentes com relação às suas atribuições. Caros amigos, a execução dos pontos do programa prepara-nos para enfrentar situações atípicas decorrentes do levantamento das variáveis envolvidas. Gostaria de enfatizar que o entendimento das metas propostas aponta para a melhoria das condições financeiras e administrativas exigidas. </p><p>É claro que o acompanhamento das preferências de consumo assume importantes posições no estabelecimento do investimento em reciclagem técnica. Ainda assim, existem dúvidas a respeito de como o novo modelo estrutural aqui preconizado faz parte de um processo de gestão da gestão inovadora da qual fazemos parte. O incentivo ao avanço tecnológico, assim como a revolução dos costumes nos obriga à análise do sistema de formação de quadros que corresponde às necessidades. </p>"
]

var ocupacao = [0.6, 0.23, 0.47, 1]

var horarios = ["11:00", "12:20", "13:50", "14:10", "15:00", "15:45", "16:15", "17:40", "18:05", "19:55", "20:15", "21:30", "22:45", "23:58", "00:35", "01:15"]

function parseURLParams(url) {
    var queryStart = url.indexOf("?") + 1,
        queryEnd   = url.indexOf("#") + 1 || url.length + 1,
        query = url.slice(queryStart, queryEnd - 1),
        pairs = query.replace(/\+/g, " ").split("&"),
        parms = {}, i, n, v, nv;

    if (query === url || query === "") {
        return;
    }

    for (i = 0; i < pairs.length; i++) {
        nv = pairs[i].split("=");
        n = decodeURIComponent(nv[0]);
        v = decodeURIComponent(nv[1]);

        if (!parms.hasOwnProperty(n)) {
            parms[n] = [];
        }

        parms[n].push(nv.length === 2 ? v : null);
    }
    return parms;
}

function imprimeFilme(dia, filme) {
    document.write("<a href='horarios.html?dia=" + dia + "&filme=" + filme + "'>" + filmesNames[filme] + "</a> <br />");
}

function nomeDia(n) {
    var d = new Date();
    if (n == d.getDay()) return "Hoje";
    else return dayNames[n];
}