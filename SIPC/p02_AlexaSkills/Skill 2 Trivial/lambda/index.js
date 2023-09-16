/* *
 * This sample demonstrates handling intents from an Alexa skill using the Alexa Skills Kit SDK (v2).
 * Please visit https://alexa.design/cookbook for additional examples on implementing slots, dialog management,
 * session persistence, api calls, and more.
 * */
const Alexa = require('ask-sdk-core');

const LaunchRequestHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'LaunchRequest';
    },
    handle(handlerInput) {
        initData();
        const questionText = getQuestion();
        currentStatus = 'Question';
        const speakOutput = 'Hola! Vamos a jugar a un TRIVIAL, Tendrás que responder \
        diciendo la respuesta correcta a la pregunta que se te haga, dicho esto, ¡empezemos!\
        ' + questionText;

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

const AnswerIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'AnswerIntent';
    },
    handle(handlerInput) {
        const AnswerValue = handlerInput.requestEnvelope.request.intent.slots.numberSlot.value;
        let speakOutput = '';
        if (currentStatus === 'Continue') {
            speakOutput += 'Responde sí o no';
        }
        else {
            if (AnswerValue === currentIndex.year) {
                speakOutput += ', ¡has acertado genial!, la respuesta era: ' + currentIndex.answer + '.';
                hits++;
            }
            else  {
                speakOutput += 'has fallado lo siento, la respuesta correcta era ' +  currentIndex.year+ ' porque ' + currentIndex.answer + '.';
            }
        }
        currentIndex = null;
        speakOutput += ', ¿Continuamos? ';
        currentStatus = 'Continue';
        
        if (exit) {
            return handlerInput.responseBuilder
                .speak(speakOutput)
                .getResponse();
        } 
        else {
            return handlerInput.responseBuilder
                .speak(speakOutput)
                .reprompt(speakOutput)
                .getResponse();
        }
    }
};

const RepeatIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.RepeatIntent';
    },
    handle(handlerInput) {
        let speakOutput = '';
        if (currentStatus === 'Question') {
            speakOutput += '¡Repetimos! ' + getQuestion(false);
        }
        else if (currentStatus === 'Continue') {
            speakOutput += '¿Continuamos? ';
        }

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};


const YesIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.YesIntent';
    },
    handle(handlerInput) {
        const speakOutput = getQuestion();
        currentStatus = 'Question';


        if (exit) {
            return handlerInput.responseBuilder
                .speak(speakOutput)
                .withShouldEndSession(true)
                .getResponse();
        } 
        else {
            return handlerInput.responseBuilder
                .speak(speakOutput)
                .reprompt(speakOutput)
                .getResponse();
        }
    }
};

const PendingIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'PendingIntent';
    },
    handle(handlerInput) {
        let speakOutput = '';
        if (pending === null) {
            if (currentIndex !== null && currentStatus === 'Question') {
                speakOutput += 'Dejaremos esta preguna pendiente para más tarde, así podrás pensarla mejor'; 
                pending = currentIndex;
            }
            speakOutput += '¡No hay preguntas pendientes!, ¿Quieres una nueva pregunta?';
            currentStatus = 'Continue';
        }
        else {
            if (currentIndex !== null && currentStatus === 'Question') {
                let tmpIndex = currentIndex;
                currentIndex = pending;
                pending = currentIndex;
                speakOutput += ' Dejaremos esta preguna pendiente para más tarde, así podrás pensarla mejor'; 
            }
            else {
                currentIndex = pending;
                pending = null;
            }
            
            speakOutput += ', ¡Ahora vamos con la pregunta que dejaste sin responder antes!, ' + getQuestion(false);
            currentStatus = 'Question';
        }


        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

const NextIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.NextIntent';
    },
    handle(handlerInput) {
        let speakOutput = '';
        if (pending !== null) {
            speakOutput = 'Tienes el máximo de preguntas pendientes sin responder, intenta responderla ahora. ';
            const tmpIndex = currentIndex;
            currentIndex = pending;
            pending = tmpIndex;
            speakOutput += getQuestion(false);
        }
        else {
            speakOutput = 'Guardemos esta pregunta, ¡vamos con la siguiente!, ';
            pending = currentIndex;
            speakOutput += getQuestion();
        }
        currentStatus = 'Question';
        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

const ClueIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'ClueIntent';
    },
    handle(handlerInput) {
        let speakOutput = '';
        if (currentStatus === 'Question') {
            speakOutput += ', ¡Ahí va una pista!, ' + currentIndex.clue + '. Te vuelvo a repetir la pregunta, ' + getQuestion(false);
        }
        else if (currentStatus === 'Continue') {
            speakOutput += 'Responde Sí o No.';
        }
        
        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

const HelpIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.HelpIntent';
    },
    handle(handlerInput) {
        const speakOutput = 'El juego consiste en que te iré haciendo preguntas de diferentes temas y tendrás que contestar \
        correctamente, pero si no sabes la respuesta puedes decirme que pase a la \
        siguiente y así tendrás tiempo de pensar la respuesta. Puedes tener hasta una pregunta pendiente de responder.';

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

const CancelAndStopIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && (Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.CancelIntent'
                || Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.StopIntent'
                || Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.NoIntent');
    },
    handle(handlerInput) {
        const speakOutput = 'Acertaste ' + hits + ' de ' + count + ', ¡preguntas. Espero que vuelvas a jugar pronto!';
        return handlerInput.responseBuilder
            .speak(speakOutput)
            .getResponse();
    }
};
/* *
 * FallbackIntent triggers when a customer says something that doesn’t map to any intents in your skill
 * It must also be defined in the language model (if the locale supports it)
 * This handler can be safely added but will be ingnored in locales that do not support it yet 
 * */
const FallbackIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.FallbackIntent';
    },
    handle(handlerInput) {
        const speakOutput = 'Lo siento, no entiendo lo que me dices. Por favor inténtalo otra vez.';

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};
/* *
 * SessionEndedRequest notifies that a session was ended. This handler will be triggered when a currently open 
 * session is closed for one of the following reasons: 1) The user says "exit" or "quit". 2) The user does not 
 * respond or says something that does not match an intent defined in your voice model. 3) An error occurs 
 * */
const SessionEndedRequestHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'SessionEndedRequest';
    },
    handle(handlerInput) {
        console.log(`~~~~ Session ended: ${JSON.stringify(handlerInput.requestEnvelope)}`);
        // Any cleanup logic goes here.
        return handlerInput.responseBuilder.getResponse(); // notice we send an empty response
    }
};
/* *
 * The intent reflector is used for interaction model testing and debugging.
 * It will simply repeat the intent the user said. You can create custom handlers for your intents 
 * by defining them above, then also adding them to the request handler chain below 
 * */
const IntentReflectorHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest';
    },
    handle(handlerInput) {
        const intentName = Alexa.getIntentName(handlerInput.requestEnvelope);
        const speakOutput = `You just triggered ${intentName}`;

        return handlerInput.responseBuilder
            .speak(speakOutput)
            //.reprompt('add a reprompt if you want to keep the session open for the user to respond')
            .getResponse();
    }
};
/**
 * Generic error handling to capture any syntax or routing errors. If you receive an error
 * stating the request handler chain is not found, you have not implemented a handler for
 * the intent being invoked or included it in the skill builder below 
 * */
const ErrorHandler = {
    canHandle() {
        return true;
    },
    handle(handlerInput, error) {
        const speakOutput = 'Lo siento, tuve problemas para hacer lo que me pediste. Inténtalo de nuevo.';
        console.log(`~~~~ Error handled: ${JSON.stringify(error)}`);

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};



let questionsList, currentIndex, count, hits, pending, currentStatus, exit;
function initData() {
    //questionsList = require('./question-list');
    questionsList = {
        '0' : {
            'id' : '0',
            'question' : 'terminó la segunda guerra mundial',
            'year' : '1945',
            'answer' : 'En 1945, los aliados llegaron al bunker de berlin y este se suicidó, finalizando la segunda guerra mundial',
            'clue' : 'en ese año, la Unión Soviética reconoce al nuevo régimen prosoviético de Polonia'
        },
        '1' : {
            'id' : '1',
            'question' : 'empezó la segunda guerra mundial',
            'year' : '1939',
            'answer' : 'El 1 de septiempbre de 1939, alemania invadió polonia y fue el detonante de la segunda guerra mundial',
            'clue' : 'en ese año terminó la guerra civil española'
        },
        '2' : {
            'id' : '2',
            'question' : 'comenzó la primera guerra mundial',
            'year' : '1914',
            'answer' : 'En 1914, Francisco Fernando Carlos Luis José María de Austria o de Habsburgo-Este fue asesinado y dió comienzo la primera guerra mundial',
            'clue' : 'En ese año fue asesinado un archiduque europeo muy importante'
        },
        '3' : {
            'id' : '3',
            'question' : 'acabó la primera guerra mundial',
            'year' : '1918',
            'answer' : 'tras terminar la guerra, 4 de las 5 potencias del mundo pasaron de tener reyes a tener democracias, sólo sobrevivió inglaterra',
            'clue' : 'fue un año después de la revolución zarista'
        }
    };
    currentIndex = null;
    count = 0;
    hits = 0;
    pending = null;
    currentStatus = null;
    exit = false;
}



function getRandomItem(obj) {
    if (Object.keys(obj).length === 0) {
        return null;
    }
    currentIndex =  obj[Object.keys(obj)[Math.floor(Math.random()*Object.keys(obj).length)]];
    return currentIndex;
}

function getQuestion(random = true) {
    let speechText = '';
    if (random) {
        speechText = getRandomItem(questionsList);
        if (currentIndex === null && pending === null) {
            const speakOutput = 'Acertaste ' + hits + ' de ' + count + ', ¡preguntas. Espero que vuelvas a jugar pronto!';
            exit = true;
            return speakOutput;
        }
        else if (currentIndex === null) {
            return 'Ya no te quedan más preguntas nuevas, pero sí te queda una pendiente, vamos con ella.' + '¿En qué año ' + speechText.question + '? ';
        }
        delete questionsList[currentIndex.id];
        count++;
    }
    else {
        speechText = currentIndex;
    }
    const speakOutput = '¿En qué año ' + speechText.question + '? ';
    return speakOutput
}

/**
 * This handler acts as the entry point for your skill, routing all request and response
 * payloads to the handlers above. Make sure any new handlers or interceptors you've
 * defined are included below. The order matters - they're processed top to bottom 
 * */
exports.handler = Alexa.SkillBuilders.custom()
    .addRequestHandlers(
        LaunchRequestHandler,
        AnswerIntentHandler,
        RepeatIntentHandler,
        NextIntentHandler,
        ClueIntentHandler,
        HelpIntentHandler,
        YesIntentHandler,
        PendingIntentHandler,
        CancelAndStopIntentHandler,
        FallbackIntentHandler,
        SessionEndedRequestHandler,
        IntentReflectorHandler)
    .addErrorHandlers(
        ErrorHandler)
    .withCustomUserAgent('sample/hello-world/v1.2')
    .lambda();