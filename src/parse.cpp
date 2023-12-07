#include "Libs.hpp"

/// @brief Parse une commande pour en extraire les channels et les mots de passe
/// @param channels Le string contenant les channels et les mots de passes delimites par ','.
/// Exemple: #channel1 password,#channel2 :password with spaces,#channelwithoutpass
/// @return Une map contenant le nom des channels et les mots de passe
std::map<std::string, std::string>	parseChannels(std::string channels)
{
	std::istringstream					sstream(channels);
	std::string							token;
	std::map<std::string, std::string>	chanMap;

	// Creer un string (token) pour chaque delimiteur (,)
	while (std::getline(sstream, token, ','))
	{
		// Cherche un delimiteur de mot de passe
		size_t		passDelim = (token.find(':') != std::string::npos) ? token.find(':') : token.find(' ');

		std::string	channelName = (passDelim != std::string::npos) ? token.substr(0, passDelim) : token; // Si aucun delimiteur, la ligne au complet est le channel
		std::string	password = (passDelim != std::string::npos) ? token.substr(passDelim + 1) : ""; // Si aucun delimiteur, pas de mot de passe

		channelName.erase(channelName.find_last_not_of(" ") + 1); // Retire les espaces a la fin du string (trouve le dernier char n'etant pas un espace)

		chanMap[channelName] = password;
	}
	return chanMap;
}
