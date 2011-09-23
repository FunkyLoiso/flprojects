#include "Player.h"

#include "GameState\Move.h"
#include "GameState\Field.h"

#include "Exception\NotInitializedException.h"
#include "Exception\TypeMismatchException.h"

using namespace AutoBot;

void Player::Init(PlayerImp* imp, const RulesManager* rulesManager) throw(TypeMismatchException)
{
	checkImplementation(imp);
	imp->SetRulesManager(rulesManager);

	m_imp = imp;
	m_inited = true;
}

Move Player::GetMove(const Field& field) const throw(NotInitializedException)
{
	if(!m_inited) throw NotInitializedException("Init");
	return m_imp->GetMove(field);
}