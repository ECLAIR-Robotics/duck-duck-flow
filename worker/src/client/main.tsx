import React from 'react';
import { hydrateRoot } from 'react-dom/client';
import App from './App';
import './index.css';
import { createBrowserRouter } from "react-router";
import { RouterProvider } from "react-router/dom";
import SensorPage from '../components/sensor/SensorPage';

const router = createBrowserRouter([
  {
    path: "/",
    Component: App,
  },
  {
    path: "/id/:id",
    Component: SensorPage,
  }
]);

// Hydrate the server-rendered content
hydrateRoot(document.getElementById('root')!, <RouterProvider router={router} />);
