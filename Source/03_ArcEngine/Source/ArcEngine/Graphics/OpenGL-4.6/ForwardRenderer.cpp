//#include "ForwardRenderer.h"
//#include <glm/ext/vector_float3.hpp>
//
//void ForwardRenderer::RenderSceneCB(const glm::mat4& projection, const glm::mat4x4& view, const glm::vec3& cameraPosition, const bool& _DebugMode, const glm::vec2& _DepthRange)
//{
//	//// Rendering commands
//	//frameBuffer->Bind();
//
//	//renderer->Render();
//
//	//glEnable(GL_DEPTH_TEST);
//	//glDepthFunc(GL_LESS);
//
//	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	//glm::mat4 projection = camera->GetProjectionMatrix();
//	//glm::mat4x4 view = camera->GetViewMatrix();
//
//	//PBRTextureShader.Use();
//
//	//PBRTextureShader.setMat4("view", view);
//	//PBRTextureShader.setMat4("projection", projection);
//
//	//glActiveTexture(GL_TEXTURE0);
//	//glBindTexture(GL_TEXTURE_2D, BC);
//	//glActiveTexture(GL_TEXTURE1);
//	//glBindTexture(GL_TEXTURE_2D, N);
//	//glActiveTexture(GL_TEXTURE2);
//	//glBindTexture(GL_TEXTURE_2D, ORM);
//
//	//PBRTextureShader.setVec3("camPos", camera->GetPosition());
//
//	//for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
//	//{
//	//	glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
//	//	newPos = lightPositions[i];
//	//	PBRTextureShader.setVec3(std::string("lightPositions[" + std::to_string(i) + "]").c_str(), newPos);
//	//	PBRTextureShader.setVec3(std::string("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);
//
//	//	auto model = glm::mat4(1.0f);
//	//	model = glm::translate(model, newPos);
//	//	model = glm::scale(model, glm::vec3(0.5f));
//	//	PBRTextureShader.setMat4("model", model);
//	//	PBRTextureShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
//	//	Cube.Draw(PBRTextureShader);
//	//}
//
//	//PBRTextureShader.setMat4("model", glm::mat4(1.0f));
//	//Rat.Draw(PBRTextureShader);
//
//	//PBRTextureShader.setMat4("model", glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 1.0f, 100.0f)));
//	//Cube.Draw(PBRTextureShader);
//
//
//	////sorted.clear();
//
//	////for (unsigned int i = 0; i < amount; i++)
//	////{
//	////	float distance = glm::length(camera->GetPosition() - glm::vec3(modelMatrices[i][3]));
//	////	sorted[distance] = modelMatrices[i];
//	////}
//
//	////for (std::map<float, glm::mat4>::iterator it = sorted.begin(); it != sorted.end(); ++it)
//	////{
//	////	lightingShader.setMat4("model", it->second);
//	////	backpack.Draw(lightingShader);
//	////}
//
//	////glm::mat4 model = glm::mat4(1.0f);
//	////model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
//	////model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
//	////blinnPhongShader.setVec3("cameraPos", camera->GetPosition());
//
//	//////blinnPhongShader.setFloat("time", glfwGetTime());
//
//	////glActiveTexture(GL_TEXTURE0);
//	////glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.textureID);
//	////planet.Draw(blinnPhongShader);
//
//	////normals.Use();
//	////normals.setMat4("model", model);
//	////normals.setMat4("view", view);
//	////normals.setMat4("projection", projection);
//	////normals.setVec3("cameraPos", camera->GetPosition());
//
//	////planet.Draw(normals);
//
//	////glActiveTexture(GL_TEXTURE0);
//	////glBindTexture(GL_TEXTURE_2D, DiffuseAsteroid);
//
//	//// draw meteorites
//	////for (unsigned int i = 0; i < amount; i++)
//	////{
//	////	lightingShader.setMat4("model", modelMatrices[i]);
//	////	asteroid.Draw(lightingShader);
//	////}
//
//
//	//PBRInstancedShader.Use();
//
//	//PBRShader.setMat4("view", view);
//	//PBRShader.setMat4("projection", projection);
//
//	//PBRShader.setVec3("albedo", glm::vec3(1.0f, 1.0f, 1.0f));
//	//PBRShader.setFloat("metallic", 0.5f);
//	//PBRShader.setFloat("ao", 1.0f);
//	//PBRShader.setFloat("roughness", 0.5f);
//
//	//PBRShader.setVec3("camPos", camera->GetPosition());
//
//	//for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
//	//{
//	//	glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
//	//	newPos = lightPositions[i];
//	//	PBRShader.setVec3(std::string("lightPositions[" + std::to_string(i) + "]").c_str(), newPos);
//	//	PBRShader.setVec3(std::string("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);
//
//	//	//auto model = glm::mat4(1.0f);
//	//	//model = glm::translate(model, newPos);
//	//	//model = glm::scale(model, glm::vec3(0.5f));
//	//	//PBRShader.setMat4("model", model);
//	//	//PBRShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
//	//	//Cube.Draw(PBRShader);
//	//}
//
//	//for (unsigned int i = 0; i < asteroid.GetMeshCount(); i++)
//	//{
//	//	glBindVertexArray(asteroid.GetMeshes()->at(i).GetVAO());
//	//	glDrawElementsInstanced(GL_TRIANGLES, asteroid.GetMeshes()->at(i).indices.size(), GL_UNSIGNED_INT, 0, amount);
//	//}
//
//	////for (int i = 0; i < amount; i++)
//	////{
//	////	lightingShader.setMat4("model", modelMatrices.at(i));
//	////	backpack.Draw(lightingShader);
//	////}
//
//	////glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
//	////glStencilMask(0x00); // disable writing to the stencil buffer
//	////glDisable(GL_DEPTH_TEST);
//
//	////outlineShader.use();
//
//	////outlineShader.setMat4("projection", projection);
//	////outlineShader.setMat4("view", view);
//
//	////for (int i = 0; i < rats; i++)
//	////{
//	////	outlineShader.setMat4("model", glm::scale(ratMats.at(i), glm::vec3(1.1f)));
//	////	backpack.Draw(outlineShader);
//	////}
//
//	////glStencilMask(0xFF);
//	////glStencilFunc(GL_ALWAYS, 1, 0xFF);
//	////glEnable(GL_DEPTH_TEST);
//
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//	//lightCubeShader.Use();
//	//lightCubeShader.setMat4("projection", projection);
//	//lightCubeShader.setMat4("view", view);
//	//for (int i = 0; i < pointLightPositions->length(); i++)
//	//{
//	//	glm::mat4 model = glm::mat4(1.0f);
//	//	model = glm::translate(model, pointLightPositions[i] * 10.0f);
//	//	model = glm::scale(model, glm::vec3(0.1f)); // a smaller cube
//
//	//	lightCubeShader.setMat4("model", model);
//
//	//	backpack.Draw(lightCubeShader);
//	//}
//
//	//glDepthFunc(GL_LEQUAL);
//	//skyboxShader.Use();
//	//skyboxShader.setMat4("projection", projection);
//	//skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
//
//	//glBindVertexArray(skyboxVAO);
//	//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.textureID);
//
//	//invertedCube.Draw(skyboxShader);
//	//glDepthFunc(GL_LESS);
//
//	//glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer->textureColorbuffer);
//	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
//	//glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
//
//	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	//glDisable(GL_DEPTH_TEST);
//
//	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//	//glClear(GL_COLOR_BUFFER_BIT);
//
//	//screenShader.Use();
//	//glBindVertexArray(quadVAO);
//	//glActiveTexture(GL_TEXTURE0);
//	//glBindTexture(GL_TEXTURE_2D, screenTexture);
//	//glDrawArrays(GL_TRIANGLES, 0, 6);
//}
//
//void ForwardRenderer::Initialize(const glm::vec2& _size)
//{
//}
//
//void ForwardRenderer::Resize(const glm::vec2& _size)
//{
//}
