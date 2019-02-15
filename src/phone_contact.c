#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "day_night.h"
#include "event_data.h"
#include "international_string_util.h"
#include "match_call.h"
#include "phone_contact.h"
#include "text.h"
#include "string_util.h"
#include "constants/day_night.h"
#include "constants/flags.h"

extern const u8 gTrainerClassNames[][13];

static bool8 CanAcceptRematch_Always(s8 dayOfWeek, s8 hour);
static bool8 CanAcceptRematch_Never(s8 dayOfWeek, s8 hour);
static bool8 CanAcceptRematch_MondayDaytime(s8 dayOfWeek, s8 hour);

static const u8 *SelectMessage_StandardMatchCallTrainer(const struct PhoneContact *phoneContact, bool8 isCallingPlayer);
static const u8 *SelectMessage_Test(const struct PhoneContact *phoneContact, bool8 isCallingPlayer);
static const u8 *SelectMessage_Mom(const struct PhoneContact *phoneContact, bool8 isCallingPlayer);
static const u8 *SelectMessage_Elm(const struct PhoneContact *phoneContact, bool8 isCallingPlayer);
static const u8 *SelectMessage_Rose(const struct PhoneContact *phoneContact, bool8 isCallingPlayer);

static const u8 sPhoneContactName_Mom[] = _("MOM");
static const u8 sPhoneContactName_ProfessorElm[] = _("PROF. ELM");

const struct PhoneContact gPhoneContacts[PHONE_CONTACT_COUNT] =
{
    [PHONE_CONTACT_MOM] ={
        .customDisplayName = sPhoneContactName_Mom,
        .selectMessage = SelectMessage_Mom,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_MOM,
        .rematchTrainerId = 0xFF,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = TRUE,
    },
    [PHONE_CONTACT_ELM] ={
        .customDisplayName = sPhoneContactName_ProfessorElm,
        .selectMessage = SelectMessage_Elm,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_ELM,
        .rematchTrainerId = 0xFF,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = TRUE,
    },
    [PHONE_CONTACT_ROSE] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_Rose,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_ROSE,
        .rematchTrainerId = REMATCH_TRAINER_ROSE,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_ANDRES] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_ANDRES,
        .rematchTrainerId = REMATCH_TRAINER_ANDRES,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_DUSTY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_DUSTY,
        .rematchTrainerId = REMATCH_TRAINER_DUSTY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_LOLA] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_LOLA,
        .rematchTrainerId = REMATCH_TRAINER_LOLA,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_RICKY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_RICKY,
        .rematchTrainerId = REMATCH_TRAINER_RICKY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_LILA_AND_ROY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_LILA_AND_ROY,
        .rematchTrainerId = REMATCH_TRAINER_LILA_AND_ROY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_CRISTIN] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_CRISTIN,
        .rematchTrainerId = REMATCH_TRAINER_CRISTIN,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_BROOKE] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_BROOKE,
        .rematchTrainerId = REMATCH_TRAINER_BROOKE,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_WILTON] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_WILTON,
        .rematchTrainerId = REMATCH_TRAINER_WILTON,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_VALERIE] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_VALERIE,
        .rematchTrainerId = REMATCH_TRAINER_VALERIE,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_CINDY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_CINDY,
        .rematchTrainerId = REMATCH_TRAINER_CINDY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_THALIA] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_THALIA,
        .rematchTrainerId = REMATCH_TRAINER_THALIA,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_JESSICA] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_JESSICA,
        .rematchTrainerId = REMATCH_TRAINER_JESSICA,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_WINSTON] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_WINSTON,
        .rematchTrainerId = REMATCH_TRAINER_WINSTON,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_STEVE] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_STEVE,
        .rematchTrainerId = REMATCH_TRAINER_STEVE,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_TONY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_TONY,
        .rematchTrainerId = REMATCH_TRAINER_TONY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_NOB] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_NOB,
        .rematchTrainerId = REMATCH_TRAINER_NOB,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_KOJI] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_KOJI,
        .rematchTrainerId = REMATCH_TRAINER_KOJI,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_FERNANDO] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_FERNANDO,
        .rematchTrainerId = REMATCH_TRAINER_FERNANDO,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_DALTON] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_DALTON,
        .rematchTrainerId = REMATCH_TRAINER_DALTON,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_BERNIE] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_BERNIE,
        .rematchTrainerId = REMATCH_TRAINER_BERNIE,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_ETHAN] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_ETHAN,
        .rematchTrainerId = REMATCH_TRAINER_ETHAN,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_JOHN_AND_JAY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_JOHN_AND_JAY,
        .rematchTrainerId = REMATCH_TRAINER_JOHN_AND_JAY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_JEFFREY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_JEFFREY,
        .rematchTrainerId = REMATCH_TRAINER_JEFFREY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_CAMERON] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_CAMERON,
        .rematchTrainerId = REMATCH_TRAINER_CAMERON,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_JACKI] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_JACKI,
        .rematchTrainerId = REMATCH_TRAINER_JACKI,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_WALTER] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_WALTER,
        .rematchTrainerId = REMATCH_TRAINER_WALTER,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_KAREN] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_KAREN,
        .rematchTrainerId = REMATCH_TRAINER_KAREN,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_JERRY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_JERRY,
        .rematchTrainerId = REMATCH_TRAINER_JERRY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_ANNA_AND_MEG] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_ANNA_AND_MEG,
        .rematchTrainerId = REMATCH_TRAINER_ANNA_AND_MEG,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_ISABEL] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_ISABEL,
        .rematchTrainerId = REMATCH_TRAINER_ISABEL,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_MIGUEL] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_MIGUEL,
        .rematchTrainerId = REMATCH_TRAINER_MIGUEL,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_TIMOTHY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_TIMOTHY,
        .rematchTrainerId = REMATCH_TRAINER_TIMOTHY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_SHELBY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_SHELBY,
        .rematchTrainerId = REMATCH_TRAINER_SHELBY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_CALVIN] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_CALVIN,
        .rematchTrainerId = REMATCH_TRAINER_CALVIN,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_ELLIOT] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_ELLIOT,
        .rematchTrainerId = REMATCH_TRAINER_ELLIOT,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_ISAIAH] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_ISAIAH,
        .rematchTrainerId = REMATCH_TRAINER_ISAIAH,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_MARIA] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_MARIA,
        .rematchTrainerId = REMATCH_TRAINER_MARIA,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_ABIGAIL] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_ABIGAIL,
        .rematchTrainerId = REMATCH_TRAINER_ABIGAIL,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_DYLAN] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_DYLAN,
        .rematchTrainerId = REMATCH_TRAINER_DYLAN,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_KATELYN] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_KATELYN,
        .rematchTrainerId = REMATCH_TRAINER_KATELYN,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_BENJAMIN] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_BENJAMIN,
        .rematchTrainerId = REMATCH_TRAINER_BENJAMIN,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_PABLO] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_PABLO,
        .rematchTrainerId = REMATCH_TRAINER_PABLO,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_NICOLAS] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_NICOLAS,
        .rematchTrainerId = REMATCH_TRAINER_NICOLAS,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_ROBERT] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_ROBERT,
        .rematchTrainerId = REMATCH_TRAINER_ROBERT,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_LAO] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_LAO,
        .rematchTrainerId = REMATCH_TRAINER_LAO,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_CYNDY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_CYNDY,
        .rematchTrainerId = REMATCH_TRAINER_CYNDY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_MADELINE] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_MADELINE,
        .rematchTrainerId = REMATCH_TRAINER_MADELINE,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_JENNY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_JENNY,
        .rematchTrainerId = REMATCH_TRAINER_JENNY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_DIANA] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_DIANA,
        .rematchTrainerId = REMATCH_TRAINER_DIANA,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_AMY_AND_LIV] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_AMY_AND_LIV,
        .rematchTrainerId = REMATCH_TRAINER_AMY_AND_LIV,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_ERNEST] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_ERNEST,
        .rematchTrainerId = REMATCH_TRAINER_ERNEST,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_CORY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_CORY,
        .rematchTrainerId = REMATCH_TRAINER_CORY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_EDWIN] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_EDWIN,
        .rematchTrainerId = REMATCH_TRAINER_EDWIN,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_LYDIA] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_LYDIA,
        .rematchTrainerId = REMATCH_TRAINER_LYDIA,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_ISAAC] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_ISAAC,
        .rematchTrainerId = REMATCH_TRAINER_ISAAC,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_GABRIELLE] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_GABRIELLE,
        .rematchTrainerId = REMATCH_TRAINER_GABRIELLE,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_CATHERINE] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_CATHERINE,
        .rematchTrainerId = REMATCH_TRAINER_CATHERINE,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_JACKSON] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_JACKSON,
        .rematchTrainerId = REMATCH_TRAINER_JACKSON,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_HALEY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_HALEY,
        .rematchTrainerId = REMATCH_TRAINER_HALEY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_JAMES] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_JAMES,
        .rematchTrainerId = REMATCH_TRAINER_JAMES,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_TRENT] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_TRENT,
        .rematchTrainerId = REMATCH_TRAINER_TRENT,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_SAWYER] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_SAWYER,
        .rematchTrainerId = REMATCH_TRAINER_SAWYER,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_KIRA_AND_DAN] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_StandardMatchCallTrainer,
        .canAcceptRematch = CanAcceptRematch_Always,
        .registeredFlag = FLAG_PHONE_CARD_KIRA_AND_DAN,
        .rematchTrainerId = REMATCH_TRAINER_KIRA_AND_DAN,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_WALLY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_Test,
        .canAcceptRematch = CanAcceptRematch_Never,
        .registeredFlag = FLAG_PHONE_CARD_WALLY,
        .rematchTrainerId = REMATCH_TRAINER_WALLY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_ROXANNE] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_Test,
        .canAcceptRematch = CanAcceptRematch_Never,
        .registeredFlag = FLAG_PHONE_CARD_ROXANNE,
        .rematchTrainerId = REMATCH_TRAINER_ROXANNE,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_BRAWLY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_Test,
        .canAcceptRematch = CanAcceptRematch_Never,
        .registeredFlag = FLAG_PHONE_CARD_BRAWLY,
        .rematchTrainerId = REMATCH_TRAINER_BRAWLY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_WATTSON] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_Test,
        .canAcceptRematch = CanAcceptRematch_Never,
        .registeredFlag = FLAG_PHONE_CARD_WATTSON,
        .rematchTrainerId = REMATCH_TRAINER_WATTSON,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_FLANNERY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_Test,
        .canAcceptRematch = CanAcceptRematch_Never,
        .registeredFlag = FLAG_PHONE_CARD_FLANNERY,
        .rematchTrainerId = REMATCH_TRAINER_FLANNERY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_NORMAN] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_Test,
        .canAcceptRematch = CanAcceptRematch_Never,
        .registeredFlag = FLAG_PHONE_CARD_NORMAN,
        .rematchTrainerId = REMATCH_TRAINER_NORMAN,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_WINONA] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_Test,
        .canAcceptRematch = CanAcceptRematch_Never,
        .registeredFlag = FLAG_PHONE_CARD_WINONA,
        .rematchTrainerId = REMATCH_TRAINER_WINONA,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_TATE_AND_LIZA] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_Test,
        .canAcceptRematch = CanAcceptRematch_Never,
        .registeredFlag = FLAG_PHONE_CARD_TATE_AND_LIZA,
        .rematchTrainerId = REMATCH_TRAINER_TATE_AND_LIZA,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_JUAN] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_Test,
        .canAcceptRematch = CanAcceptRematch_Never,
        .registeredFlag = FLAG_PHONE_CARD_JUAN,
        .rematchTrainerId = REMATCH_TRAINER_JUAN,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_SIDNEY] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_Test,
        .canAcceptRematch = CanAcceptRematch_Never,
        .registeredFlag = FLAG_PHONE_CARD_SIDNEY,
        .rematchTrainerId = REMATCH_TRAINER_SIDNEY,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_PHOEBE] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_Test,
        .canAcceptRematch = CanAcceptRematch_Never,
        .registeredFlag = FLAG_PHONE_CARD_PHOEBE,
        .rematchTrainerId = REMATCH_TRAINER_PHOEBE,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_GLACIA] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_Test,
        .canAcceptRematch = CanAcceptRematch_Never,
        .registeredFlag = FLAG_PHONE_CARD_GLACIA,
        .rematchTrainerId = REMATCH_TRAINER_GLACIA,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_DRAKE] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_Test,
        .canAcceptRematch = CanAcceptRematch_Never,
        .registeredFlag = FLAG_PHONE_CARD_DRAKE,
        .rematchTrainerId = REMATCH_TRAINER_DRAKE,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
    [PHONE_CONTACT_WALLACE] = {
        .customDisplayName = NULL,
        .selectMessage = SelectMessage_Test,
        .canAcceptRematch = CanAcceptRematch_Never,
        .registeredFlag = FLAG_PHONE_CARD_WALLACE,
        .rematchTrainerId = REMATCH_TRAINER_WALLACE,
        .availability = PHONE_AVAILABILITY_ALWAYS,
        .isPermanent = FALSE,
    },
};

static const u8 *SelectMessage_StandardMatchCallTrainer(const struct PhoneContact *phoneContact, bool8 isCallingPlayer)
{
    int rematchTrainerId = gRematchTable[phoneContact->rematchTrainerId].trainerIds[0];
    SelectMatchCallMessage(rematchTrainerId, gStringVar4, isCallingPlayer);
    return gStringVar4;
}

static const u8 *SelectMessage_Test(const struct PhoneContact *phoneContact, bool8 isCallingPlayer)
{
    static const u8 sTestText[] = _("TEST PHONE MESSAGE.");
    return sTestText;
}

static const u8 *SelectMessage_Mom(const struct PhoneContact *phoneContact, bool8 isCallingPlayer)
{
    static const u8 sTestText_Mom[] = _("Hi I'm MOM!\nBye.");
    return sTestText_Mom;
}

static const u8 *SelectMessage_Elm(const struct PhoneContact *phoneContact, bool8 isCallingPlayer)
{
    static const u8 sTestText_Elm[] = _("Hi I'm PROF. ELM!.");
    return sTestText_Elm;
}

static const u8 *SelectMessage_Rose(const struct PhoneContact *phoneContact, bool8 isCallingPlayer)
{
    static const u8 sTestText_Rose[] = _("Hi I'm ROSE!\nI'm being forced to call you!");
    if (FlagGet(FLAG_FORCE_PHONE_CALL_ROSE) && isCallingPlayer)
        return sTestText_Rose;
    else
        return SelectMessage_StandardMatchCallTrainer(phoneContact, isCallingPlayer);
}

static bool8 CanAcceptRematch_Always(s8 dayOfWeek, s8 hour)
{
    return TRUE;
}

static bool8 CanAcceptRematch_Never(s8 dayOfWeek, s8 hour)
{
    return FALSE;
}

static bool8 CanAcceptRematch_MondayDaytime(s8 dayOfWeek, s8 hour)
{
    return dayOfWeek == DAY_MONDAY
        && GetTimeOfDay(hour) == TIME_DAY;
}

bool8 IsPhoneContactAvailable(const struct PhoneContact *phoneContact, s8 dayOfWeek, s8 hour)
{
    switch (phoneContact->availability)
    {
    case PHONE_AVAILABILITY_ALWAYS:
        return TRUE;
    default:
        return TRUE;
    }
}

static const u8 sPhoneContactName_UnknownContact[] = _("UNKNOWN CONTACT");

const u8 *BuildPhoneContactDisplayName(const struct PhoneContact *phoneContact, u8 *dest)
{
    int i, j;
    int classXOffset;
    const u8 *src;

    if (phoneContact->customDisplayName)
    {
        return phoneContact->customDisplayName;
    }
    else if (phoneContact->rematchTrainerId != 0xFF)
    {
        int trainerId = gRematchTable[phoneContact->rematchTrainerId].trainerIds[0];
        src = gTrainers[trainerId].trainerName;
        i = 0;
        for (j = 0; src[j] != EOS; j++)
            dest[i++] = src[j];

        dest[i++] = CHAR_COLON;
        dest[i++] = EXT_CTRL_CODE_BEGIN;
        dest[i++] = EXT_CTRL_CODE_SIZE;
        dest[i++] = 0;

        classXOffset = GetStringRightAlignXOffset(0, gTrainerClassNames[gTrainers[trainerId].trainerClass], 128);
        dest[i++] = EXT_CTRL_CODE_BEGIN;
        dest[i++] = EXT_CTRL_CODE_CLEAR_TO;
        dest[i++] = classXOffset;

        src = gTrainerClassNames[gTrainers[trainerId].trainerClass];
        for (j = 0; src[j] != EOS; j++)
            dest[i++] = src[j];

        dest[i++] = EOS;
        return dest;
    }
    else
    {
        return sPhoneContactName_UnknownContact;
    }
}
