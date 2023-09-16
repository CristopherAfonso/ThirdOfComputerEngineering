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
        initData(handlerInput);
        const questionText = getQuestion(handlerInput);
        const locale = handlerInput.requestEnvelope.request.locale
        currentStatus = 'Question';
        let speakOutput = 'Hola! Vamos a jugar a ... ¿En qué año pasó? ... Tendrás que responder \
        diciendo qué año corresponde con el hito al que hago referencia. ... Vamos a empezar! ...\
        ' + questionText;
        if (locale === 'en-US') {
            speakOutput = 'Hello! Lets play... What year did it happen? ... You will have to respond \
            by saying which year corresponds to the milestone I refer to. ... Lets start! ... \
        ' + questionText;
        }

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
        const locale = handlerInput.requestEnvelope.request.locale;
        let speakOutput = '';
        if (locale === 'en-US') {
            speakOutput += "You answered " + AnswerValue;
            if (currentStatus === 'Continue') {
                speakOutput += 'Answer yes or no';
            } else {
                if (AnswerValue === currentIndex.year){
                    speakOutput += '. Correct answer, the year ' + currentIndex.year + ' is true because ' + currentIndex.answer;
                    hits++;
                } else {
                    speakOutput += '. Incorrect answer, the right answer is ' + currentIndex.year + ' because ' + currentIndex.answer;
                }
            }
            speakOutput += '...Shall we continue?';
        } else {
            speakOutput += "Respondiste " + AnswerValue;
            if (currentStatus === 'Continue') {
                speakOutput += 'Responde si o no';
            } else {
                if (AnswerValue === currentIndex.year){
                    speakOutput += '. Respuesta correcta, el año ' + currentIndex.year + ' es verdadero porque ' + currentIndex.answer;
                    hits++;
                } else {
                    speakOutput += '. Respuesta incorrecta, el año correcto es ' + currentIndex.year + ' porque ' + currentIndex.answer;
                }
            }
            speakOutput += '...Continuamos?';
        }
        currentStatus = 'Continue';
        currentIndex = null;
        if (exit) {
            return handlerInput.responseBuilder
                .speak(speakOutput)
                .getResponse();
        } 
        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    },
};

const YesIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.YesIntent';
    },
    handle(handlerInput) {
        const speakOutput = getQuestion(handlerInput);
        currentStatus = 'Question';
        if (exit) {
            return handlerInput.responseBuilder
                .speak(speakOutput)
                .withShouldEndSession(true)
                .getResponse();
        } else {
            return handlerInput.responseBuilder
                .speak(speakOutput)
                .reprompt(speakOutput)
                .getResponse();
        }
    }
};

const ExitHandler = {
    canHandle(handlerInput) {
        const request = handlerInput.requestEnvelope.request;
        return request.type === 'IntentRequest'
        && (request.intent.name === 'AMAZON.CancelIntent'
        || request.intent.name === 'AMAZON.NoIntent'
        || request.intent.name === 'AMAZON.StopIntent');
    },
    handle(handlerInput) {
        const requestAttributes = handlerInput.attributesManager.getRequestAttributes();
        const locale = handlerInput.requestEnvelope.request.locale;
        let speakOutput = 'Has conseguido acertar ' + hits + ' de ' + count + ' preguntas, ... hasta luego!';
        if (locale === 'en-US') {
            speakOutput = 'You have got ' + hits + ' of the ' + count + ' questions right, ... see you later!';
        }

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .getResponse();
    },
};

const RepeatIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.RepeatIntent';
    },
    handle(handlerInput) {
        let speakOutput = '';
        const locale = handlerInput.requestEnvelope.request.locale;
        if (currentStatus === 'Question') {
            if (locale === 'en-US') {
                speakOutput += 'Repeating! ... ' + getQuestion(handlerInput, false);
            } else {
                speakOutput += 'Repetimos! ... ' + getQuestion(handlerInput, false);
            }
        } else if (currentStatus === 'Continue') {
            if (locale === 'en-US') {
                speakOutput += 'Shall we continue! ... ' + getQuestion(handlerInput, false);
            } else {
                speakOutput += 'Continuamos? ';
            }
        }

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
        const locale = handlerInput.requestEnvelope.request.locale;
        if (currentStatus === 'Question') {
            if (locale === 'en-US') {
                speakOutput += 'There goes a clue! ... ' + currentIndex.clue + '. ... I repeat the question again. ... ' + getQuestion(handlerInput, false);
            } else {
                speakOutput += 'Ahí va una pista! ... ' + currentIndex.clue + '. ... Te vuelvo a repetir la pregunta. ... ' + getQuestion(handlerInput, false);
            }
        }
        else if (currentStatus === 'Continue') {
            if (locale === 'en-US') {
                speakOutput += 'Answer Yes or No.';
            } else {
                speakOutput += 'Responde Sí o No.';
            }
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
        const locale = handlerInput.requestEnvelope.request.locale;
        if (pending !== null) {
            if (locale === 'en-US') {
                speakOutput = 'You reached the maximum number of questions pending to be answered, lets go for it again. ... ' + getQuestion(handlerInput, false);
            } else {
                speakOutput = 'Alcanzaste el máximo de preguntas pendientes de responder, vamos a por ella de nuevo. ... ' + getQuestion(handlerInput, false);
            }
            const tmpIndex = currentIndex;
            currentIndex = pending;
            pending = tmpIndex;
            speakOutput += getQuestion(handlerInput, false);
        }
        else {
            if (locale === 'en-US') {
                speakOutput = 'We save this question for later, lets go with the next one! ...';
            } else {
                speakOutput = 'Guardamos esta pregunta para después, vamos con la siguiente! ... ';
            }
            pending = currentIndex;
            speakOutput += getQuestion(handlerInput);
        }
        currentStatus = 'Question';
        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

const PendingIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'PendingIntent';
    },
    handle(handlerInput) {
        let speakOutput = '';
        const locale = handlerInput.requestEnvelope.request.locale;
        if (locale === 'en-US') {
            if (pending === null) {
                if (currentIndex !== null && currentStatus === 'Question') {
                    speakOutput += 'We have left this question unanswered, we are saving it for later... ';
                    pending = currentIndex;
                }
                speakOutput += 'You have no pending questions! ... Do you want to continue with a new question?';
                currentStatus = 'Continue';
            }
            else {
                if (currentIndex !== null && currentStatus === 'Question') {
                    let tmpIndex = currentIndex;
                    currentIndex = pending;
                    pending = currentIndex;
                    speakOutput += 'We have left this question unanswered, saving it for later...'; 
                }
                else {
                    currentIndex = pending;
                    pending = null;
                }
                
                speakOutput += 'Lets go with the question we had pending! ... ' + getQuestion(handlerInput, false);
                currentStatus = 'Question';
            }  
        } else {
            if (pending === null) {
                if (currentIndex !== null && currentStatus === 'Question') {
                    speakOutput += 'Hemos dejado esta pregunta sin responder, la guardamos para después ... '; 
                    pending = currentIndex;
                }
                speakOutput += 'No tienes preguntas pendientes! ... Quieres continuar con una nueva pregunta?';
                currentStatus = 'Continue';
            }
            else {
                if (currentIndex !== null && currentStatus === 'Question') {
                    let tmpIndex = currentIndex;
                    currentIndex = pending;
                    pending = currentIndex;
                    speakOutput += 'Hemos dejado esta pregunta sin responder, la guardamos para después ... '; 
                }
                else {
                    currentIndex = pending;
                    pending = null;
                }
                
                speakOutput += 'Vamos con la pregunta que teníamos pendiente! ... ' + getQuestion(handlerInput, false);
                currentStatus = 'Question';
            }
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
        const locale = handlerInput.requestEnvelope.request.locale;
        let speakOutput = 'El juego consiste en que te iré haciendo preguntas y tendrás que acertar el año \
        correcto, pero si no sabes la respuesta puedes decirme que pase a la \
        siguiente y así tendrás tiempo de pensar la respuesta. Puedes tener hasta una pregunta pendiente de responder.';
        if (locale === 'en-US') {
            speakOutput = 'The game consists in that I will ask you questions and you will have to guess the correct \
             year, but if you dont know the answer you can tell me to go to the \
             next and so you will have time to think about the answer. You can have up to one question to answer.';
        }

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
        const locale = handlerInput.requestEnvelope.request.locale;
        let speakOutput = 'Has conseguido acertar ' + hits + ' de ' + count + ' preguntas. ... Hasta luego!';
        if (locale === 'en-US') {
            speakOutput = 'You managed to hit ' + hits + ' of ' + count + ' questions. ... Bye!';
        }
        return handlerInput.responseBuilder
            .speak(speakOutput)
            .getResponse();
    }
};

const FallbackIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.FallbackIntent';
    },
    handle(handlerInput) {
        const locale = handlerInput.requestEnvelope.request.locale;
        let speakOutput = 'Lo siento, no entiendo lo que me dices. Por favor inténtalo otra vez.';
        if (locale === 'en-US') {
            speakOutput = 'Sorry, I dont understand what youre saying. Please try again.';
        }

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

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

const ErrorHandler = {
    canHandle() {
        return true;
    },
    handle(handlerInput, error) {
        const speakOutput = 'Sorry, I had trouble doing what you asked. Please try again.';
        console.log(`~~~~ Error handled: ${JSON.stringify(error)}`);

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

let questionsList, currentIndex, count, hits, pending, currentStatus, exit;
function initData(handlerInput) {
    const locale = handlerInput.requestEnvelope.request.locale;
    questionsList = require('./question-list');
    if (locale === 'en-US') {
        questionsList = require('./question-list2');
    }
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

function getQuestion(handlerInput, random = true) {
    const locale = handlerInput.requestEnvelope.request.locale;
    let speechText = '';
    if (random) {
        speechText = getRandomItem(questionsList);
        if (currentIndex === null && pending === null) {
            let speakOutput = 'Ya respondiste todas las preguntas! ... Has conseguido acertar ' + hits + ' de ' + count + ' preguntas. ... Hasta luego!';
            if (locale === 'en-US') {
                speakOutput = 'You already answered all the questions! ... You got ' + hits + ' of the ' + count + ' questions right. ... Bye!';
            }
            exit = true;
            currentIndex = null;
            count = 0;
            hits = 0;
            pending = null;
            currentStatus = null;
            return speakOutput;
        }
        else if (currentIndex === null) {
            if (locale === 'en-US') {
                return 'You dont have any more new questions, but if you have one pending, lets go with it. ... ' + 'What year ' + speechText.question + '? ';
            } else {
                return 'Ya no te quedan más preguntas nuevas, pero sí te queda una pendiente, vamos con ella. ... ' + '¿En qué año ' + speechText.question + '? ';
            }
        }
        delete questionsList[currentIndex.id];
        count++;
    }
    else {
        speechText = currentIndex;
    }
    let speakOutput = '¿En qué año ' + speechText.question + '? ';
    if (locale === 'en-US') {
        speakOutput = 'What year ' + speechText.question + '? ';
    }
    return speakOutput
}

exports.handler = Alexa.SkillBuilders.custom()
    .addRequestHandlers(
        LaunchRequestHandler,
        AnswerIntentHandler,
        HelpIntentHandler,
        RepeatIntentHandler,
        NextIntentHandler,
        ClueIntentHandler,
        PendingIntentHandler,
        YesIntentHandler,
        ExitHandler,
        FallbackIntentHandler,
        SessionEndedRequestHandler)
    .addErrorHandlers(
        ErrorHandler)
    .withCustomUserAgent('sample/hello-world/v1.2')
    .lambda();