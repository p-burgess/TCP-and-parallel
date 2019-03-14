%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

tic,   % time computation start


%____________________System Parameters_____________________

n=8;             %Number of sub-carriers
M=4;             %Modulation level
alpha=1;         %Bandwidth compression factor

Rs=1e2;         %Input symbols data rate
Ts=1/Rs;        %Input symbol duration
T=n*Ts;         %SEFDM symbol duration

%____________________Simulation Parameters___________________

Oversampling=2;
NoSamples=Oversampling*n;    %Number of samples per SEFDM frame
Tstep=T/NoSamples;           %Sampling period
fs=1/Tstep;                  %Sampling frequency
t=(0:Tstep:T-Tstep);         %Generating time vector, will determine number of time samples
a=alpha*1/T;                 %fractional operator ,Fraction of frequency seperation
normalize=1/sqrt(NoSamples); %Normalization factor

data_size=n*500;             %Number of all transmitted symbols
No_blocks=data_size/n;       %Number of transmitted SEFDM symbols   

EbNo_start=2
EbNo_final=5;


%____Initialize variables
IP_BLOCK=zeros(n,1);
ber_ZF_candidate=zeros(n,EbNo_final);
SYMBOL_MODI = zeros(NoSamples,1);
%X = zeros(n,classname): returns an n-by-n array of zeros of data type classname.

%__________________________Generating Data____________________

X = randint(data_size, 1, M);   %MATLAB function: generating random numbers
%                                                   between 0-(M-1)
%OUT = RANDINT(M,N,IRANGE) generates an M-by-N matrix of random integers.
%   IRANGE can be either a scalar or a two-element vector:

IP_SIG=qammod(X,M);             %MATLAB function: Performing M-QAM
%                                                 modulation
%y = qammod(x,M) outputs the complex envelope y of the modulation of the 
%    message signal x using quadrature amplitude modulation. M is the 
%    alphabet size and must be an integer power of 2. The message signal 
%    must consist of integers between 0 and M-1. The signal constellation
%    is rectangular or cross-shaped, and the nearest pair of points in the
%    constellation is separated by 2. If x is a matrix with multiple rows,
%    the function processes the columns independently.
% 
%  After the coding operation, the data are modulated. A data rate of 
%  3.807Gb/s is achieved by employing a 16 Quadrature Amplitude Modulation
%  (QAM) - Orthogonal Frequency-Division Multiplexing (OFDM) modulation
%  (WirelessHD, 2009). The bits are grouped in symbols of four bits each,
%  having values between 0 and 24-1=15. These data are sent to the symbol
%  mapper, which maps the input bits into 16QAM Gray-coded symbols. The
%  16QAM mapper and demapper can be implemented by means of the functions
%  qammod and qamdemod available in MATLAB®.

%__________________________Generating carriers__________________

 for l=1:n
        CARRIERS((1:1:length(t)),l)=normalize*exp(1i*2*pi*a*(l-1)*t);                  
 end

 %__________________________ correlation mtrix__________________

    correlation=CARRIERS'*CARRIERS;
         
 %________________________________________________________________________
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %________________________Starting Transmission___________________________

 for EbNo=EbNo_start:1: EbNo_final

%   Eb_No =  EbNo  

  for k=1:No_blocks
      
      IP_BLOCK(1:n,1)=IP_SIG((k-1)*n+1:(k-1)*n+n); % Construct IP symbols vector, this vector will be modulated
      SEFDM_SYMBOL=CARRIERS*IP_BLOCK;              % SEFDM modulation
      SYMBOL_MODI((k-1)*NoSamples+1:(k-1)*NoSamples+NoSamples) = SEFDM_SYMBOL;
 
  end

%     Samples_size = length(SYMBOL_MODI);
    %_________________________Generating AWGN Noise____________________
    sigPower=SYMBOL_MODI'*SYMBOL_MODI;
    No=sigPower/(10^((EbNo)/10)*log2(M)*data_size);
    sigma = sqrt(No/2)
    awgnNoise=sigma*randn(Oversampling*length(IP_SIG),1)+1i*sigma*randn(Oversampling*length(IP_SIG),1); % MTALB function (randn) : generate random symbols
 
  for k=1:No_blocks
       
      IP_BLOCK(1:n,1)=IP_SIG((k-1)*n+1:(k-1)*n+n);
      
      %%%%__________________AWGN Channel____________________________________
      
      SEFDM_SYMBOL_NOISY1 = SYMBOL_MODI((k-1)*NoSamples+1:(k-1)*NoSamples+NoSamples) + awgnNoise((k-1)*NoSamples+1:(k-1)*NoSamples+NoSamples);
 

      %%%_________________ SEFDM demodulation ________________________________________
      
      SYM_RX=CARRIERS'*SEFDM_SYMBOL_NOISY1;   


      %%%%%  4QAM/QPSK zero forcing detector %%%%%%%%%%%%%%%%%%
      %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      SYM_RX_EVEN = correlation\SYM_RX; % MATLAB function : inverse of correlation matrix; then multiply with SYM_RX

      SYM_RX_EVEN_REAL=real(SYM_RX_EVEN)>0; % MATLAB function : extract real part
      SYM_RX_EVEN_IMAG=imag(SYM_RX_EVEN)>0; % MATLAB function : extract imag part
      
      SYM_RX_EVEN_REAL=SYM_RX_EVEN_REAL-(SYM_RX_EVEN_REAL==0);
      SYM_RX_EVEN_IMAG=SYM_RX_EVEN_IMAG-(SYM_RX_EVEN_IMAG==0);
      
      ZF =SYM_RX_EVEN_REAL+1i*SYM_RX_EVEN_IMAG;

      %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
     

       numZF=symerr(IP_BLOCK,ZF,'row-wise'); % MATLAB function : compare IP_BLOCK and ZF, line by line
       ber_ZF_candidate(:,EbNo) = ber_ZF_candidate(:,EbNo) +  numZF/No_blocks  ;
             
  end
  
 end


 
 %______Calculate overall BER___________________________________
 ber_mean_ZF_candidate(:,1)=mean(ber_ZF_candidate)/log2(M) % MATLAB function (mean): get average value column by column
   

 
 %%%%%%________________________4QAM Theoretical result______________ 
Eb_N0_dB00=1:1:10;
theoryBer = 0.5*erfc(sqrt(10.^(Eb_N0_dB00/10))); 

%%%%________________Generating figures

 figure();

 semilogy(theoryBer,'k--');
 hold on;
 semilogy(ber_mean_ZF_candidate,'-p');
 title(['SEFDM BER vs Eb/No for \alpha=', num2str(alpha),' N= ', num2str(n), ' and M = ', num2str(M)]);
 legend('Single Carrier','ZF',3);
 xlabel('E_b/N_0 [dB]');
 ylabel('BER');  
 grid on;
 
 
toc, % time computation finish



