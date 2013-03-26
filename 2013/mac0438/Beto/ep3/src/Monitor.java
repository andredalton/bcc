import java.util.concurrent.Semaphore;

public class Monitor extends Thread {
	// Variaveis permanentes
	private int tempo; // Tempo desde o começo da execução
	private int nPessoasElevador; // Número de pessoas no elevador
	private int capacidadeTotal;
	private int andarAtual;
	private int nAndares;
	private int entraramRodada; // Quantas pessoas entraram no elevador na rodada atual
	private int sairamRodada; // Quantas pessoas sairam do elevador na rodada atual
	private int nThreads;
	private int barreira;
	private boolean subindo;
	public Semaphore s;
	
	// Comandos de inicialização
	public Monitor(int andares, int capacidade, int nThreads) {
		this.nThreads = nThreads;
		this.nPessoasElevador = 1;
		this.capacidadeTotal = capacidade;
		this.andarAtual = 1;
		this.subindo = true;
		this.tempo = 0;
		this.nAndares = andares;
		this.entraramRodada = 0;
		this.sairamRodada = 0;
		this.s = new Semaphore(1, true);
		this.barreira = 1;
		start();
	}
	
	// Demais procedimentos
	public void run() {
		try {
			Thread.sleep(10);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	// Movimenta o elevador para a próxima rodada
	public void movimenta() {
		if (this.subindo) {
			if (this.andarAtual < this.nAndares) {
				this.andarAtual++;
			}
			else {
				this.subindo = false;
				this.andarAtual--;
			}
		}
		else {
			if (this.andarAtual > 1) {
				this.andarAtual--;
			}
			else {
				this.subindo = true;
				this.andarAtual++;
			}
		}
		this.tempo++;
		this.entraramRodada = 0;
		this.sairamRodada = 0;
		this.barreira = 1;
	}
	
	// Retorna true se todas as threads de Pessoas chegaram na barreira
	public boolean terminouThreads() {
		if (this.barreira == this.nThreads) {
			return true;
		}
		else {
			return false;
		}
	}
	
	public int getBarreira() {
		return this.barreira;
	}
	
	public void incBarreira() {
		this.barreira++;
	}
	
	public int getNPessoasElevador() {
		return this.nPessoasElevador;
	}

	// De fato coloca uma pessoa no elevador
	public void entraPessoa() {
		this.nPessoasElevador++;
		this.entraramRodada++;
	}
	
	// De fato tira uma pessoa do elevador
	public void saiPessoa() {
		this.nPessoasElevador--;
		this.sairamRodada++;
	}
	
	// Verifica se uma pessoa pode entrar no elevador
	public boolean tentaEntrarPessoa(int andarPessoa) {
		if (this.andarAtual == andarPessoa && this.nPessoasElevador < this.capacidadeTotal) {
			return true;
		}
		else {
			return false;
		}
	}
	
	// Verifica se uma pessoa pode sair do elevador
	public boolean tentaSairPessoa(int andarPessoa) {
		if (this.andarAtual == andarPessoa) {
			return true;
		}
		else {
			return false;
		}
	}
	
	public int getEntraramRodada() {
		return this.entraramRodada;
	}
	
	public int getSairamRodada() {
		return this.sairamRodada;
	}
	
	public int getTempo() {
		return this.tempo;
	}
	
	public int getNAndares() {
		return this.nAndares;
	}
	
	public int getAndarAtual() {
		return this.andarAtual;
	}
}