%% Electron fluxes

data = dlmread('e_fluxes_mx500_x1e-10.txt');
E10 = data(:,1);
ef_10 = data(:,2);
data = dlmread('e_fluxes_mx500_x1e-11.txt');
E11 = data(:,1);
ef_11 = data(:,2);
data = dlmread('e_fluxes_mx500_x1e-12.txt');
E12 = data(:,1);
ef_12 = data(:,2);
data = dlmread('e_fluxes_mx500_x1e-13.txt');
E13 = data(:,1);
ef_13 = data(:,2);



semilogy(E10,ef_10,'k', E11,ef_11,'r', E12,ef_12,'g', E13,ef_13,'b' ,'linewidth',2), hold on
axis([0 1 1e-5 1e1])
xlabel('$E/m_\chi$','interpreter','latex')
ylabel('$dN/dz$ [ann$^{-1}$]','interpreter','latex')
set(gca,'fontsize',24,'XMinorTick','on')
annotation('textbox',[0.5 0.6 .3 .3],'String','$m_\chi = 2000$ GeV',...
    'FitBoxToText','on','edgecolor','none','fontsize',20,'interpreter','latex')
annotation('textbox',[0.5 0.55 .3 .3],'String','channel: $\nu_e \bar{\nu}_e$',...
    'FitBoxToText','on','edgecolor','none','fontsize',20,'interpreter','latex')
h = legend('$x=-10$','$x=-11$','$x=-12$','$x=-13$','interpreter','latex');
set(h,'position',[0.6 0.2 0.3 0.25],'box','off','fontsize',20);