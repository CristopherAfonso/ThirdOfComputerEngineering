import cv2
import mediapipe as mp
from rich.console import Console

mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_hands = mp.solutions.hands

console = Console()

cap = cv2.VideoCapture(0)
with mp_hands.Hands(
        model_complexity=0,
        min_detection_confidence=0.5,
        min_tracking_confidence=0.5) as hands:
    while cap.isOpened():
        success, image = cap.read()
        if not success:
            print("Ignoring empty camera frame.")
            # If loading a video, use 'break' instead of 'continue'.
            continue

        # Flip the image horizontally for a selfie-view display.
        image = cv2.flip(image, 1)

        # To improve performance, optionally mark the image as not writeable to
        # pass by reference.
        image.flags.writeable = False
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        results = hands.process(image)

        # Draw the hand annotations on the image.
        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        if results.multi_hand_landmarks:
            for hand_landmarks in results.multi_hand_landmarks:
                mp_drawing.draw_landmarks(
                    image,
                    hand_landmarks,
                    mp_hands.HAND_CONNECTIONS,
                    mp_drawing_styles.get_default_hand_landmarks_style(),
                    mp_drawing_styles.get_default_hand_connections_style())

                tipIds = [4, 8, 12, 16, 20]

                lm = hand_landmarks.landmark

                # x,y coordinates of pinky tip. Coordinates are normalized to [0.0,1.0] with width and height of the image
                lm[tipIds[4]].x
                lm[tipIds[4]].y

                # height, width and depth (RGB=3) of image
                (h, w, d) = image.shape

                # OpenCV funciona para mostrar un circulo:
                # cv2.circle(image, center_coordinates, radius in pixels, color (Blue 0-255, Green 0-255, Red 0-255), thickness in pixels (-1 solid))

                # Example: draw a red solid circle of 10 pixel radius in the tip of pinky finger:
                # cv2.circle(image, (int(lm[tipIds[0]].x*w),int(lm[tipIds[0]].y*h)), 10, (0,0,255), -1)

                dedos_levantados = 0

                for i in range(1, 5):
                    if (lm[tipIds[i]].y < lm[tipIds[i] - 1].y and
                            lm[tipIds[i] - 1].y < lm[tipIds[i] - 2].y):
                        dedos_levantados += 1

                    # console.log(f"Dedos levantados: {dedos_levantados}")
                    cv2.putText(image, "Dedos levantados: " + str(dedos_levantados), (20, 400),
                                cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), thickness=2)

                    if (dedos_levantados == 1):

                        # PULGAR ABAJO
                        if (lm[4].y > lm[5].y and  # El punto 4 del pulgar tiene que ser mayor que el 5, el 5 es la base del dedo indice
                            # El punto 5 del indice tiene que ser mayor que el 9, el 9 es la base del dedo del medio que debe de ir debajo de la base del dedo indice
                            lm[5].y > lm[9].y and
                            # El punto 9 del pulgar tiene que ser mayor que el 13, la base del dedo anular debe ir debajo de la base del dedo del medio
                            lm[9].y > lm[13].y and
                            # El punto 13 del pulgar tiene que ser mayor que el 17, la base del dedo meñique debe ir debajo de la base del dedo anular
                            lm[13].y > lm[17].y and

                                    # El punto 4 del pulgar tiene que ser mayor que el 3
                                    lm[4].y > lm[3].y and
                                    # El punto 3 del pulgar tiene que ser mayor que el 2
                                lm[3].y > lm[2].y and
                                # El punto 2 del pulgar tiene que ser mayor que el 1
                                lm[2].y > lm[1].y and
                                # El punto 1 del pulgar tiene que ser menor que el 0
                                    lm[1].y > lm[0].y
                                ):

                            # Para mostrar el texto habiendo echo un gesto cv2.putText (variable, texto, donde (x,y), fuente, tamaño_fuente, color (,,), ancho_trazo)
                            cv2.putText(image, "Pulgar Abajo", (20, 60),
                                        cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), thickness=2)

                        # PULGAR ARRIBA
                        if (lm[4].y < lm[5].y and  # El punto 4 del pulgar tiene que ser menor que el 5, el 5 es la base del dedo indice
                            # El punto 5 del pulgar tiene que ser menor que el 9, el 9 es la base del dedo del medio que debe de ir encima de la base del dedo indice
                            lm[5].y < lm[9].y and
                            # El punto 9 del pulgar tiene que ser menor que el 13, la base del dedo anular debe ir encima de la base del dedo del medio
                            lm[9].y < lm[13].y and
                            # El punto 13 del pulgar tiene que ser menor que el 17, la base del dedo meñique debe ir encima de la base del dedo anular
                            lm[13].y < lm[17].y and

                                    # El punto 4 del pulgar tiene que ser menor que el 3
                                    lm[4].y < lm[3].y and
                                    # El punto 3 del pulgar tiene que ser menor que el 2
                                lm[3].y < lm[2].y and
                                # El punto 2 del pulgar tiene que ser menor que el 1
                                lm[2].y < lm[1].y and
                                # El punto 1 del pulgar tiene que ser menor que el 0
                                    lm[1].y < lm[0].y
                                ):
                            cv2.putText(image, "Pulgar Arriba", (20, 60),
                                        cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), thickness=2)

                    if (dedos_levantados == 2):
                        # PAZ
                        if (lm[8].y < lm[7].y and
                            lm[7].y < lm[6].y and
                            lm[6].y < lm[5].y and
                            lm[5].y < lm[0].y and

                                lm[12].y < lm[11].y and
                                lm[11].y < lm[10].y and
                                lm[10].y < lm[9].y and
                            lm[9].y > lm[0].y
                            ):
                            cv2.putText(
                                image, "Paz", (20, 60), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), thickness=4)

                    if (dedos_levantados == 2):
                        # MANO CORNUDA
                        if (
                            # El punto 8 del indice tiene que ser menor que el 7, el 8 es la punta del dedo indice
                            lm[8].y < lm[7].y and
                            # El punto 7 del indice tiene que ser menor que el 6, el 7 es el siguiente punto debajo de la punta del dedo indice
                            lm[7].y < lm[6].y and
                            # El punto 6 del indice tiene que ser menor que el 5, el 5 es la base del dedo indice y el 6 es el punto encima de este
                            lm[6].y < lm[5].y and

                            # La punta del pulgar debe ser mayor que la base del dedo indice
                            lm[5].y < lm[4].y and
                            # La punta del pulgar debe ser mayor que la base del dedo meñique
                            lm[17].y < lm[4].y and

                            # El punto 20 del meñique tiene que ser menor que el 19, el 20 es la punta del dedo meñique y el 19 es el punto que tiene debajo
                            lm[20].y < lm[19].y and
                            # El punto 19 del meñique tiene que ser menor que el 18, el 18 es el punto que está encima del punto que es la base del dedo
                            lm[19].y < lm[18].y and
                            # El punto 18 del meñique tiene que ser menor que el 17, el 17 es la base del dedo meñique
                            lm[18].y < lm[17].y and

                            # La punta del pulgar debe ser menor que el punto que está debajo del pulgar que es el 3
                            lm[4].y < lm[3].y and
                            # El punto 3 del pulgar debe ser menor que el punto 2 del pulgar
                            lm[3].y < lm[2].y and
                            # El punto 2 del pulgar debe ser menor que el punto 1 del pulgar
                            lm[2].y < lm[1].y and

                            # El punto 10 del dedo del medio debe ser menor que el punto 9 que es la base del dedo
                            lm[10].y < lm[9].y and
                            # El punto 10 del dedo del medio debe ser menor que el punto 11 que es el punto que viene antes de su punta
                            lm[10].y < lm[11].y and
                            # El punto 14 del dedo anular debe ser menor que el punto 13 que es la base del dedo
                            lm[14].y < lm[13].y and
                            # El punto 14 del dedo del medio debe ser menor que el punto 15 que es el punto que viene antes de su punta
                            lm[14].y < lm[15].y and

                            # La punta del pulgar debe estar en el centro de la mano y su limite es la base del dedo indice, si se aleja más no está en el centro
                            lm[5].x < lm[4].x
                        ):
                            cv2.putText(image, "Mano Cornuda", (20, 60),
                                        cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), thickness=2)

        cv2.imshow('MediaPipe Hands', image)

        if cv2.waitKey(5) & 0xFF == 27:
            break

cap.release()
