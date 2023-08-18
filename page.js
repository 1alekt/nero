"use client"
import { useState, useEffect } from "react";
import database from "./firebase";
import { ref, onValue } from "firebase/database";

export default function Home() {
  const [data, setData] = useState({});
  const [showAnimation, setShowAnimation] = useState(false);

  useEffect(() => {
    const dataRef = ref(database, "datos/");

    const handleData = (snapshot) => {
      const value = snapshot.val();
      if (value !== null) {
        setData(value);
        setShowAnimation(true); // Activamos la animación
        setTimeout(() => {
          setShowAnimation(false); // Desactivamos la animación después de un tiempo
        }, 1000); // Duración de la animación en milisegundos (1 segundo en este caso)
      } else {
        setData({});
      }
    };

    onValue(dataRef, handleData);

    return () => {
      onValue(dataRef, null);
    };
  }, []);

  return (
    <div
      className="flex items-center justify-center min-h-screen bg-gradient-to-r from-pink-500 to-purple-500 p-8"
    >
      <div className="w-full max-w-md bg-white rounded-lg shadow-lg p-4">
        <h1 className="text-5xl font-bold text-purple-800 mb-8 text-center">
          Nero01
        </h1>
        <div className="overflow-x-auto">
          <table className={`table-auto w-full text-center ${showAnimation ? 'animate-fade-in' : ''}`}>
            <thead>
              <tr className="bg-purple-600 text-white">
                <th className="px-4 py-2">PH</th>
                <th className="px-4 py-2">Temperatura</th>
              </tr>
            </thead>
            <tbody>
              <tr className="bg-purple-100 text-purple-800">
                <td className="px-4 py-2 font-semibold">{data.ph}</td>
                <td className="px-4 py-2 font-semibold">{data.temp}</td>
              </tr>
            </tbody>
          </table>
        </div>
      </div>
    </div>
  );
}